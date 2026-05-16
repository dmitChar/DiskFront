#include "mediacontroller.h"
#include <QQmlEngine>
#include <QStandardPaths>
#include <QDebug>

MediaController::MediaController(APIService *api, FileController *file, TransferModel *transfer, QObject *parent)
    : QObject{parent}, m_api(api), m_file(file), m_transfer(transfer)
{
    QString m_cacheDir = QStandardPaths::writableLocation(QStandardPaths::CacheLocation) + "/CloudDisk-cache";

    m_cache = new FileCacheManager(m_cacheDir, this);
    m_imgProvider = new MediaImageProvider(m_api->getBaseUrl(), m_api->getJwtToken(), m_cache, m_file, m_transfer);
    m_mediaProxy = new LocalMediaProxy(m_api->getBaseUrl(), this);

    if (m_mediaProxy->start())
        emit proxyReadyChanged();

    connect(m_api, &APIService::tokenChanged, this, [this] ()
    {
        m_imgProvider->setToken(m_api->getJwtToken());
        m_mediaProxy->setToken(m_api->getJwtToken());
    });
}

void MediaController::registerImageProvider(QQmlEngine *engine)
{
    engine->addImageProvider("media", m_imgProvider);
    qDebug() << "[Media] ImageProvider registred as image://media/";
}

void MediaController::onLogin(const QString &token, qint64 userId)
{
    m_token = token;
    m_userId =userId;
    m_imgProvider->setToken(token);
    m_imgProvider->setUserId(userId);
    m_mediaProxy->setToken(token);
    m_mediaProxy->setUserId(userId);
}

void MediaController::onLogout()
{
    m_token = "";
    m_userId = -1;
    m_imgProvider->setToken({});
    m_mediaProxy->setToken({});
}

bool MediaController::proxyReady() const
{
    return m_mediaProxy->isRunning();
}

// --------- QML Методы ----------

QString MediaController::imageUrl(const QString &fileId) const
{
    return "image://media/" + fileId;
}

QString MediaController::videoUrl(const QString &fileId) const
{
    return m_mediaProxy->mediaUrl(fileId);
}

void MediaController::fetchText(const QString &fileId)
{
    // Проверка в кеше
    auto cached = m_cache->get(fileId.toLongLong(), m_userId);
    if (cached)
    {
        emit textReady(fileId, QString::fromUtf8(cached.value()));
        return;
    }

    // Запрос на сервер через APIService
    auto reply = m_api->downloadFile(fileId.toLongLong(), m_userId);

    if (reply)
    {
        auto file = m_file->getFileById(fileId.toLongLong());
        m_transfer->addDownload(file.name, m_file->currentPath() + "/" + file.name, reply);
    }
    connect(reply, &QNetworkReply::finished, this, [this, fileId, reply] ()
    {
        reply->deleteLater();
        if (reply->error() != QNetworkReply::NoError)
        {
            emit textError(fileId, reply->errorString());
            qDebug() << "Was emitted textError";
            return;
        }
        QByteArray data = reply->readAll();
        m_cache->put(fileId.toLongLong(), m_userId, data, "text/plain");
        emit textReady(fileId, QString::fromUtf8(data));
        qDebug() << "Was emitted textReady";
    });
}

void MediaController::invalidate(const QString &fileId)
{
    m_cache->remove(fileId.toLongLong(), m_userId);
    qDebug() << "[Media] Invalidated" << fileId;
}

void MediaController::clearCache()
{
    m_cache->clear();
}

double MediaController::cacheSize() const
{
    return (m_cache->memoryCacheUsedBytes() + m_cache->diskCacheUsedBytes() / (1024.0 * 1024.0));
}


