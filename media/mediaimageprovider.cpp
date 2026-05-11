#include "mediaimageprovider.h"
#include "QNetworkReply"

MediaImageProvider::MediaImageProvider(const QString &baseUrl, const QString jwtToken, FileCacheManager *cache)
    : QQuickImageProvider(QQuickImageProvider::Image)
    , m_baseUrl(baseUrl)
    , m_token(jwtToken)
    , m_cache(cache)
{

}


// ── Основной метод ────────────────────────────────────────────────────────────
// Qt вызывает его из QThreadPool — НЕ из main-треда.
// Поэтому создаём локальный QNetworkAccessManager
QImage MediaImageProvider::requestImage(const QString &id, QSize *size, const QSize &requestedSize)
{
    QString fileId = id.split('?').first();

    // ----- L1-L2 кеш ------
    auto cached = m_cache->get(fileId.toLongLong(), m_userId);
    if (cached)
    {
        QImage img;
        if (img.loadFromData(cached.value()))
        {
            if (size)
                *size = img.size();
            if (requestedSize.isValid())
            {
                return img.scaled(requestedSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
            }
            return img;
        }
        // Данные в кеше повреждены — удаляем и запрашиваем заново
        m_cache->remove(fileId.toLongLong(), m_userId);
    }

    // ----- Сеть ------
    QImage img = fetchFromNetwork(fileId);
    if (size)
        *size = img.size();

    if (!img.isNull() && requestedSize.isValid())
    {
        return img.scaled(requestedSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    }
    return img;
}

QImage MediaImageProvider::fetchFromNetwork(const QString &fileId)
{
    // Локальный NAM — QQuickImageProvider вызывается из пула тредов,
    // у каждого треда свой NAM во избежание гонок
    QNetworkAccessManager nam;
    QString token;
    {
        QMutexLocker lock(&m_tokenMutex);
        token = m_token;
    }

    QUrl url(m_baseUrl + "/api/files/" + fileId + "/download");
    QNetworkRequest req(url);
    req.setRawHeader("Authorization", ("Bearer " + token).toUtf8());
    // Отключение кеша Qt
    req.setAttribute(QNetworkRequest::CacheLoadControlAttribute, QNetworkRequest::AlwaysNetwork);

    // TO DO Пока что синхронный запрос через QEventLoop
    QEventLoop loop;
    QNetworkReply *reply = nam.get(req);
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);

    loop.exec();
    reply->deleteLater();

    if (reply->error() != QNetworkReply::NoError)
    {
        qWarning() << "[ImageProvider] Network error for" << fileId << reply->errorString();
        return errorImage();
    }

    int httpCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if (httpCode != 200 && httpCode != 206)
    {
        qWarning() << "[ImageProvider] HTTP" << httpCode << "for" << fileId;
        return errorImage();
    }

    QByteArray data = reply->readAll();
    if (data.isEmpty())
        return errorImage();

    QString mime = reply->header(QNetworkRequest::ContentTypeHeader).toString();

    // Сохранение в кеш
    m_cache->put(fileId.toLongLong(), m_userId, data, mime);

    QImage img;
    if (!img.loadFromData(data))
    {
        qWarning() << "[ImageProvider] Failed to decode image for" << fileId;
        return errorImage();
    }

    qDebug() << "[ImageProvider] Fetched" << fileId << img.size() << data.size() << "bytes";
    return img;
}

void MediaImageProvider::setToken(const QString &token)
{
    QMutexLocker locker(&m_tokenMutex);
    m_token = token;
}

void MediaImageProvider::setUserId(qint64 userId)
{
    m_userId = userId;
}


// Возвращает прозрачный 1×1 пиксель при ошибке.
// QML Image покажет ошибку загрузки, но не упадёт.
QImage MediaImageProvider::errorImage()
{
    QImage img(1, 1, QImage::Format_ARGB32);
    img.fill(Qt::transparent);
    return img;
}
