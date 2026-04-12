#include "apiservice.h"
#include <QNetworkReply>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QUrlQuery>
#include <QUrl>

APIService::APIService(QObject *parent)
    : QObject(parent), m_netManager(new QNetworkAccessManager)
{
}

//--------------- Service ----------------

void APIService::setBaseUrl(const QString &newBaseUrl)
{
    m_baseUrl = newBaseUrl.endsWith('/') ? newBaseUrl.chopped(1) : newBaseUrl;
    emit baseUrlChanged();
}

void APIService::setToken(const QString &newToken)
{
    m_token = newToken;
    emit tokenChanged();
}

//--------------- Configure HTTP request ----------------

QNetworkRequest APIService::makeRequest(const QString &endpoint, const QUrlQuery &query = QUrlQuery()) const
{
    QUrl url(m_baseUrl + endpoint);
    if (!query.isEmpty())
        url.setQuery(query);

    QNetworkRequest req(url);
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    if (!m_token.isEmpty())
        req.setRawHeader("Authorization", "Bearer " + m_token.toUtf8());

    req.setAttribute(QNetworkRequest::RedirectPolicyAttribute, QNetworkRequest::NoLessSafeRedirectPolicy);

    qDebug() << "[ApiService] makeRequest" << url <<  "with token:" << m_token;

    return req;
}

//--------------- Reading server's reply ----------------

void APIService::handleReply(QNetworkReply *reply, ApiCallback cb)
{
    connect(reply, &QNetworkReply::finished, this, [reply, cb, this] ()
    {
        reply->deleteLater();

        // Чтение ответа сервера
        QByteArray data = reply->readAll();
        int httpCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

        // Парсинг данных в json
        QJsonParseError error;
        QJsonDocument doc = QJsonDocument::fromJson(data, &error);
        if (error.error != QJsonParseError::NoError)
        {
            cb(ApiResponse::fail("Invalid JSON: " + error.errorString(), httpCode));
            return;
        }

//        // Проверка на ошибки в ответе сервера
//        if (reply->error() != QNetworkReply::NoError)
//        {
//            emit networkError(reply->errorString());
//            qDebug() << "[ApiService] HandleReply error:" << reply->errorString() << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
//            cb(ApiResponse::fail(reply->errorString(), reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt()));
//            return;
//        }

        // Парсинг json'a
        QJsonObject root = doc.object();
        bool success = root["success"].toBool();

        // Выполнилась ли операция на сервере успешно
        if (!success)
        {
            QString msg = root["error"].toObject()["message"].toString("Unknown error");
            qDebug() << "[ApiService] HandleReply error:" << reply->errorString() << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() << "| message:" << msg;

            cb(ApiResponse::fail(msg, httpCode));
            return;
        }

        QString message = root["message"].toString();

        if (root["data"].isArray())
        {
            ApiResponse r = ApiResponse::okArray(root["data"].toArray(), message);
            r.httpCode = httpCode;
            cb(r);
        }
        else
        {
            ApiResponse r = ApiResponse::ok(root["data"].toObject(), message);
            r.httpCode = httpCode;
            cb(r);
        }

    });

}


//--------------- Basic Senders ----------------

void APIService::sendPost(const QString &endpoint, const QJsonObject &body, ApiCallback cb)
{
    qDebug() << "[ApiService] Sending Post";
    auto *reply = m_netManager->post(makeRequest(endpoint), QJsonDocument(body).toJson(QJsonDocument::Compact));
    handleReply(reply, std::move(cb));
}

void APIService::sendGet(const QString &endpoint, const QUrlQuery &query, ApiCallback cb)
{
    auto *reply = m_netManager->get(makeRequest(endpoint, query));
    handleReply(reply, std::move(cb));
}

//--------------- Auth api ----------------

void APIService::postLogin(const QString &login, const QString &passwd, ApiCallback cb)
{
    QJsonObject obj;
    obj["username"] = login;
    obj["password"] = passwd;

    sendPost(QString("/api/auth/login"), obj, std::move(cb));
}

void APIService::getMe(ApiCallback cb)
{
    sendGet("/api/auth/me", {}, std::move(cb));
}

void APIService::postRegister(const QString &login, const QString &email, const QString password, ApiCallback cb)
{
    QJsonObject obj;
    obj["username"] = login;
    obj["email"] = email;
    obj["password"] = password;

    sendPost("/api/auth/register", obj, std::move(cb));

}

//void APIService::getAllFiles()
//{
//    QNetworkRequest request(QUrl(""));

//    auto reply = m_netManager.get(request);

//    connect(reply, &QNetworkReply::finished, this, [=]()
//    {
//        QVector<FileInfo> files;
//        auto json = QJsonDocument::fromJson(reply->readAll());
//        auto array = json.array();

//        for (auto item : array)
//        {
//            QJsonObject obj = item.toObject();
//            FileInfo file;
//            file.id = obj["id"].toString();
//            file.name = obj["name"].toString();

//            files.append(file);
//        }
//        emit filesReceived(files);
//    });

//}


