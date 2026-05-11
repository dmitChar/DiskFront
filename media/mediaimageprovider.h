#ifndef MEDIAIMAGEPROVIDER_H
#define MEDIAIMAGEPROVIDER_H

#include <QQuickImageProvider>
#include <QNetworkAccessManager>
#include <QEventLoop>
#include <QMutex>
#include <QString>

#include "filecachemanager.h"


// Провайдер изображений для QML.
// Регистрируется как "image://media/<fileId>"
//
// QML использует:
//   Image { source: "image://media/42" }
//   Image { source: "image://media/42?t=1700000000" }

class MediaImageProvider : public QQuickImageProvider
{
public:
    explicit MediaImageProvider(const QString &baseUrl, const QString jwtToken, FileCacheManager *cache);

    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize) override;

    // Обновить токен (при смене сессии)
    void setToken(const QString &token);
    void setUserId(qint64 userId);

private:
    QImage fetchFromNetwork(const QString &fileId);
    QImage errorImage();

    QString m_baseUrl;
    QString m_token;
    qint64 m_userId = 0;
    FileCacheManager *m_cache = nullptr;
    QMutex m_tokenMutex; // защита m_token при setToken() из main-треда
};

#endif // MEDIAIMAGEPROVIDER_H
