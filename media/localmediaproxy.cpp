#include "localmediaproxy.h"

LocalMediaProxy::LocalMediaProxy(const QString &backendUrl, qint64 userId, QObject *parent):
    QObject{parent},
    m_server(new QTcpServer(this)),
    m_backendUrl(backendUrl),
    m_nam(new QNetworkAccessManager(this))

{
    connect(m_server, &QTcpServer::newConnection, this, &LocalMediaProxy::onNewConnection);
}

LocalMediaProxy::~LocalMediaProxy()
{
    stop();
}

bool LocalMediaProxy::start()
{
    if (!m_server->listen(QHostAddress::LocalHost, 0))
    {
        qWarning() << "[PROXY] Failed to start:" << m_server->errorString();
        return false;
    }
    m_port = m_server->serverPort();
    qDebug() << "[PROXY] is listening on" << m_port;
    return true;
}

bool LocalMediaProxy::stop()
{
    if (m_server->isListening())
        m_server->close();
}

QString LocalMediaProxy::mediaUrl(const QString &fileId) const
{
    return QString("http://127.0.0.1:%1/%2").arg(m_port).arg(fileId);
}

void LocalMediaProxy::setToken(const QString &newToken)
{
    QMutexLocker locker(&m_tokenMutex);
    m_token = newToken;
}

void LocalMediaProxy::setUserId(qint64 newUserId)
{
    m_userId = newUserId;
}


// -- Новое соединение от QMediaPlayer ------------

void LocalMediaProxy::onNewConnection()
{
    while (m_server->hasPendingConnections())
    {
        QTcpSocket *client = m_server->nextPendingConnection();
        m_clients[client] = ClientState{};

        connect(client, &QTcpSocket::readyRead, this, &LocalMediaProxy::onClientReadyRead);
        connect(client, &QTcpSocket::disconnected, this, &LocalMediaProxy::onClientDisconnected);
    }
}

void LocalMediaProxy::onClientReadyRead()
{
    QTcpSocket *client = qobject_cast<QTcpSocket*>(sender());
    if (!client)
    {
        qDebug() << "[PROXY] ERROR!";
        return;
    }

    // Ожидаем конца HTTP заголовков
    auto buf = m_clients[client].buffer;
    int headerEnd = buf.indexOf("\r\n\r\n");
    if (headerEnd < 0)
        return; // Еще не прочитали заголовки

    QByteArray headers = buf.left(headerEnd);
    buf.clear();

    handleRequest(client, headers);
}

void LocalMediaProxy::onClientDisconnected()
{
    auto *client = qobject_cast<QTcpSocket*>(sender());
    if (client)
    {
        m_clients.remove(client);
        client->deleteLater();
    }
}


// -- Разбор входящего HTTP-запроса ---------

void LocalMediaProxy::handleRequest(QTcpSocket *client, const QByteArray &rawHeaders)
{
    QList<QByteArray> lines = rawHeaders.split('\n');
    if (lines.isEmpty())
    {
        sendError(client, 400, "Bad request");
        return;
    }

    // Строка запроса: "GET /id HTTP/1.1"
    QString requestLine = QString::fromLatin1(lines.at(0).trimmed());
    QStringList parts = requestLine.split(' ');
    if (parts.size() < 2)
    {
        sendError(client, 400, "Bad request");
        return;
    }

    QString method = parts.at(0).trimmed().toUpper();
    QString path = parts.at(1).trimmed();

    if (method != "GET" && method != "HEAD")
    {
        sendError(client, 405, "Method is Not Allowed");
        return;
    }

    // Получение id файла
    QString fileId = path.mid(1).split('?').first();
    if (fileId.isEmpty())
    {
        sendError(client, 400, "Missing file ID");
        return;
    }

    // Поиск заголовка Range
    QString rangeHeader;
    for (int i = 1; i < lines.size(); ++i)
    {
        QString line = QString::fromLatin1(lines.at(i).trimmed());
        if (line.startsWith("Range:"), Qt::CaseInsensitive)
        {
            rangeHeader = line.mid(6).trimmed();
            break;
        }
    }

    qDebug() << "[Proxy]" << method << fileId << (rangeHeader.isEmpty() ? "" : "Range: " + rangeHeader);
    proxyToBackend(client, method, fileId, rangeHeader);
}


void LocalMediaProxy::proxyToBackend(QTcpSocket *client, const QString &method, const QString &fileId, const QString &rangeHeader)
{
    QString token;
    {
        QMutexLocker lock(&m_tokenMutex);
        token = m_token;
    }
    QUrl url = m_backendUrl + "/api/files/" + fileId + "/download";
    QNetworkRequest req(url);
    req.setRawHeader("Authorization", QString("Bearer " + token).toUtf8());

    if (!rangeHeader.isEmpty())
        req.setRawHeader("Range", rangeHeader.toLatin1());

    // Отключаем встроенный кеш Qt для прокси — кешируем на уровне MediaCache
    req.setAttribute(QNetworkRequest::CacheLoadControlAttribute, QNetworkRequest::AlwaysNetwork);

    QNetworkReply *reply = (method == "HEAD") ? m_nam->head(req) : m_nam->get(req);

    // Когда бэкенд ответил — пересылаем клиенту
    connect(reply, &QNetworkReply::finished, this, [this, client, reply] ()
    {
        reply->deleteLater();
        if (!client || m_clients.contains(client))
            return;

        if (reply->error() != QNetworkReply::NoError)
        {
            qWarning() << "[Proxy] Backend reply error:" << reply->errorString();
            sendError(client, 502, "Bad Gateway");
            return;
        }

        int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

        // Формирование ответа клиенту
        QByteArray resp;
        resp += buildStatusLine(statusCode);

        // Пробрасываем нужные заголовки от бэкенда
        const QList<QPair<QByteArray,QByteArray>> rawHdrs = reply->rawHeaderPairs();
        QStringList passthroughHeaders =
        {
            "content-type", "content-length", "content-range",
            "accept-ranges", "last-modified", "etag"
        };

        for (const auto &hdr : rawHdrs)
        {
            QString name = QString::fromLatin1(hdr.first).toLower();
            if (passthroughHeaders.contains(name))
            {
                resp += hdr.first + ": " + hdr.second + "\r\n";
            }
        }

        resp += "Connection: close\r\n";
        resp += "\r\n";

        resp += reply->readAll();

        client->write(resp);
        client->flush();
        client->disconnectFromHost();
    });
}

void LocalMediaProxy::sendError(QTcpSocket *client, int code, const QString &msg)
{
    QByteArray body = msg.toUtf8();
    QByteArray resp = buildStatusLine(code);
    resp += "Content-Type: text/plain\r\n";
    resp += "Content-Length: " + QByteArray::number(body.size()) + "\r\n";
    resp += "Connection: close\r\n\r\n";

    resp+= body;
    client->write(resp);
    client->flush();
    client->disconnectFromHost();
}


QByteArray LocalMediaProxy::buildStatusLine(int code)
{
    static const QMap<int,QByteArray> texts =
    {
        {200,"OK"},{206,"Partial Content"},{400,"Bad Request"},
        {401,"Unauthorized"},{403,"Forbidden"},{404,"Not Found"},
        {405,"Method Not Allowed"},{416,"Range Not Satisfiable"},{502,"Bad Gateway"}
    };
    return "HTTP/1.1 " + QByteArray::number(code) + " " + texts.value(code, "Unknown") + "\r\n";
}

