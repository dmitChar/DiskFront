#ifndef APISERVICE_H
#define APISERVICE_H

#include <QObject>
#include <QNetworkAccessManager>
#include <functional>
#include <optional>

#include "filecachemanager.h"
#include "Utils/apiresponse.h"
#include "Utils/fileitem.h"

using std::optional;
using std::nullopt;

using ApiCallback = std::function<void(ApiResponse)>;

class APIService : public QObject
{
    Q_OBJECT
public:
    explicit APIService(QObject *parent = nullptr);
    QString getBaseUrl() const { return m_baseUrl; }
    QString getJwtToken() const { return m_token; }

    //---- Auth api ------
    void postLogin(const QString & login, const QString &passwd, ApiCallback cb);
    void postLogout(ApiCallback cb);
    void getMe(ApiCallback cb);
    void postRegister(const QString &login, const QString &email, const QString password, ApiCallback cb);

    //---- FILES API ------
    void getFiles(const QString &path, ApiCallback cb);
    void getFileInfo(const QString &path, ApiCallback cb);
    void postMkdir(const QString &path, ApiCallback cb);
    void postCopy(const QString &from, const QString &to, ApiCallback cb);
    void postMove(const QString &from, const QString &to, ApiCallback cb);
    void postRenameFile(const QString &path, const QJsonObject &body, ApiCallback cb);
    void deleteItem(qint64 fileId, ApiCallback cb);
    void getUserQuota(ApiCallback cb);
    QNetworkReply *uploadFile(const QString &serverDir, const QString &localPath, const QString &fileName, const QByteArray &data);
    QNetworkReply *downloadFile(qint64 fileId, qint64 userId);

    //---- Service ------
    void setBaseUrl(const QString &newBaseUrl);
    void setToken(const QString &newToken);
    QNetworkRequest makeRequest(const QString &endpoint, const QUrlQuery &query) const;


private:
    //----- Basic Senders ------
    void sendPost(const QString &endpoint, const QJsonObject &body, ApiCallback cb);
    void sendGet(const QString &endpoint, const QUrlQuery &query, ApiCallback cb);
    void sendDelete(const QString &endpoint, const QUrlQuery &q, ApiCallback cb);
    void sendPut(const QString &endpoint, const QString &q, const QJsonObject &body, ApiCallback cb);

    //---- Service ------
    void handleReply(QNetworkReply *reply, ApiCallback cb);

    //----- Auth Endpoints ------

private:
    QNetworkAccessManager *m_netManager;
    FileCacheManager *m_cache;

    QString m_baseUrl;
    QString m_token;

signals:
    void filesReceived(QVector<FileItem> files);
    void networkError(const QString &error);
    void tokenChanged();
    void baseUrlChanged();
    void fileReady(qint64 id, QString path);

};

#endif // APISERVICE_H
