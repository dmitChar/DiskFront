#ifndef MEDIACONTROLLER_H
#define MEDIACONTROLLER_H

#include <QObject>
#include <QString>
#include <QStandardPaths>
#include "filecachemanager.h"
#include "media/localmediaproxy.h"
#include "media/mediaimageprovider.h"
#include "apiservice.h"


// Точка входа для всей медиа-подсистемы.
// Регистрируется в QML как "Media".
//
// QML использует:
//   Image       { source: Media.imageUrl("69") }
//   MediaPlayer { source: Media.videoUrl("69") }
//   Text        { text:   ""  }  // + Connections { target: Media; function onTextReady(id,text){} }

class MediaController : public QObject
{
    Q_OBJECT
public:
    explicit MediaController(APIService *api, QObject *parent = nullptr);

    // Вызывается после создания QQmlEngine — регистрирует ImageProvider
    void registerImageProvider(QQmlEngine *engine);

    // Обновить токен и userId после логина / смены сессии
    void onLogin (const QString &token, qint64 userId);
    void onLogout();

    bool proxyReady() const;

signals:

private:
    APIService          *m_api = nullptr;
    FileCacheManager    *m_cache = nullptr;
    MediaImageProvider  *m_imgProvider = nullptr;
    LocalMediaProxy     *m_mediaProxy = nullptr;

    QString m_token;
    qint64 m_userId = -1;

};

#endif // MEDIACONTROLLER_Hs
