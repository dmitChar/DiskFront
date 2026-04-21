#include "apiservice.h"
#include <QNetworkReply>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QUrlQuery>
#include <QUrl>
#include <QHttpMultiPart>
#include <QMimeDatabase>

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

void APIService::sendDelete(const QString &endpoint, const QUrlQuery &q, ApiCallback cb)
{
    auto *reply = m_netManager->deleteResource(makeRequest(endpoint, q));
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

void APIService::postLogout(ApiCallback cb)
{
    sendPost(QString("/api/auth/logout"), {}, std::move(cb));
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

//--------------- FILES API ----------------

void APIService::getFiles(const QString &path, ApiCallback cb)
{
    QUrlQuery q;
    q.addQueryItem("path", path);
    sendGet("/api/files", q, std::move(cb));
}

void APIService::postMkdir(const QString &path, ApiCallback cb)
{
    sendPost("/api/files/mkdir", {{"path", path}}, std::move(cb));
}

void APIService::postCopy(const QString &from, const QString &to, ApiCallback cb)
{
    sendPost("/api/files/copy", {{"from", from}, {"to", to}}, std::move(cb));
}

void APIService::postMove(const QString &from, const QString &to, ApiCallback cb)
{
    sendPost("/api/files/move", {{"from", from}, {"to", to}}, std::move(cb));
}

void APIService::deleteItem(const QString &path, ApiCallback cb)
{
    QUrlQuery q;
    q.addQueryItem("path", path);
    sendDelete("/api/files", q, std::move(cb));
}

void APIService::getUserQuota(ApiCallback cb)
{
    sendGet("/api/user/quota", {}, std::move(cb));
}

void APIService::getFileInfo(const QString &path, ApiCallback cb)
{
    QUrlQuery q;
    q.addQueryItem("path", path);
    sendGet("/api/files/info", q, std::move(cb));
}

/**
 * @brief APIService::uploadFile Функция, отправляющая POST запрос для загрузки файла на сервер по указанному пути
 * @param serverPath Путь папки на сервере, в которую необходимо добавить файл
 * @param localPath Путь файла на клиенте
 * @param fileName Название файла
 * @param data Сам файл(данные)
 * @return Результат выполнения запроса
 */
QNetworkReply *APIService::uploadFile(const QString &serverDir, const QString &localPath, const QString &fileName, const QByteArray &data)
{
    QUrl url(m_baseUrl + "/api/files/upload");
    QUrlQuery q;
    q.addQueryItem("path", serverDir);
    url.setQuery(q);

    QNetworkRequest req(url);
    if (!m_token.isEmpty())
        req.setRawHeader("Authorization", "Bearer " + m_token.toUtf8());

    auto *multipart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

    QHttpPart filePart;
    filePart.setHeader(QNetworkRequest::ContentDispositionHeader, QString("form-data; name=\"file\"; filename=\"%1\"").arg(fileName));

    QMimeDatabase db;
    QString mime = db.mimeTypeForFileNameAndData(localPath, data).name();
    filePart.setHeader(QNetworkRequest::ContentTypeHeader, mime);
    filePart.setBody(data);
    multipart->append(filePart);

    auto *reply = m_netManager->post(req, multipart);
    multipart->setParent(reply);
    return reply;

}

/**
 * @brief APIService::downloadFile Функция, отправляющая GET запрос на сервер для скачивания с него файла по указанному пути
 * @param path Путь файла на сервере
 * @return Результат выполенения запроса
 */
QNetworkReply *APIService::downloadFile(const QString &path)
{
    QUrlQuery q;
    q.addQueryItem("path", path);
    return m_netManager->get(makeRequest("/api/files/download", q));
}




