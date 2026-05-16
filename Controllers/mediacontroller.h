#ifndef MEDIACONTROLLER_H
#define MEDIACONTROLLER_H

#include <QObject>
#include <QString>
#include <QStandardPaths>
#include "filecachemanager.h"
#include "media/localmediaproxy.h"
#include "media/mediaimageprovider.h"
#include "apiservice.h"
#include "models/transfermodel.h"
#include "Controllers/filecontroller.h"


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
    explicit MediaController(APIService *api, FileController *file, TransferModel* transfer, QObject *parent = nullptr);

    // Вызывается после создания QQmlEngine — регистрирует ImageProvider
    void registerImageProvider(QQmlEngine *engine);

    // Обновить токен и userId после логина / смены сессии
    void onLogin (const QString &token, qint64 userId);
    void onLogout();



    // ----- QML Методы -------
    Q_INVOKABLE bool proxyReady() const;

    // URL для Image
    Q_INVOKABLE QString imageUrl(const QString &fileId) const;

    // URL для MediaPlayer
    Q_INVOKABLE QString videoUrl(const QString &fileId) const;

    // fetch текстового файла
    Q_INVOKABLE void fetchText(const QString &fileId);

    // Принудительно убрать файл из кеша
    Q_INVOKABLE void invalidate(const QString &fileId);

    //Очистить весь кеш
    Q_INVOKABLE void clearCache();

    // Размер кеша в МБ
    Q_INVOKABLE double cacheSize() const;

signals:
    void proxyReadyChanged();

    // Ответы на fetchText()
    void textReady(const QString &fileId, const QString &text);
    void textError(const QString &fileId, const QString &error);

private:
    APIService         *m_api = nullptr;
    FileController     *m_file = nullptr;
    TransferModel      *m_transfer = nullptr;
    FileCacheManager   *m_cache = nullptr;
    MediaImageProvider *m_imgProvider = nullptr;
    LocalMediaProxy    *m_mediaProxy = nullptr;

    QString m_token;
    qint64 m_userId = -1;
    QString m_cacheDir = "";

};

#endif // MEDIACONTROLLER_Hs
