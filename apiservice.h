#ifndef APISERVICE_H
#define APISERVICE_H

#include <QObject>
#include <QNetworkAccessManager>
#include <functional>

#include "fileinfo.h"
#include "Utils/apiresponse.h"

using ApiCallback = std::function<void(ApiResponse)>;

class APIService : public QObject
{
    Q_OBJECT
public:
    explicit APIService(QObject *parent = nullptr);
    //void getAllFiles();

    //---- Auth api ------
    void postLogin(const QString & login, const QString &passwd, ApiCallback cb);
    void getMe(ApiCallback cb);
    void postRegister(const QString &login, const QString &email, const QString password, ApiCallback cb);

    //---- Service ------
    void setBaseUrl(const QString &newBaseUrl);
    void setToken(const QString &newToken);
    QNetworkRequest makeRequest(const QString &endpoint, const QUrlQuery &query) const;


private:
    //----- Basic Senders ------
    void sendPost(const QString &endpoint, const QJsonObject &body, ApiCallback cb);
    void sendGet(const QString &endpoint, const QUrlQuery &query, ApiCallback cb);

    //---- Service ------
    void handleReply(QNetworkReply *reply, ApiCallback cb);

    //----- Auth Endpoints ------

private:
    QNetworkAccessManager *m_netManager;

    QString m_baseUrl;
    QString m_token;

signals:
    void filesReceived(QVector<FileInfo> files);
    void networkError(const QString &error);
    void tokenChanged();
    void baseUrlChanged();

};

#endif // APISERVICE_H
