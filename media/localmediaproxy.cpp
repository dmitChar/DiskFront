#include "localmediaproxy.h"

LocalMediaProxy::LocalMediaProxy(const QString &backendUrl, QObject *parent):
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

static QByteArrayList normalizeLineEndings(QByteArrayList &data)
{
    QByteArrayList result{};

    for (QByteArray &line : data)
    {
        if (line.endsWith("\r"))
            line.append('\n');

        if (!line.endsWith("\r\n"))
            line.append("\r\n");
        result.append(line);
    }
    return result;
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
    //return QString("http://127.0.0.1:8080/api/files/%1/download").arg(fileId);
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
        m_buffers[client] = {};

        connect(client, &QTcpSocket::readyRead, this, &LocalMediaProxy::onClientReadyRead);
        connect(client, &QTcpSocket::disconnected, this, &LocalMediaProxy::onDisconnected);
    }
}

void LocalMediaProxy::onClientReadyRead()
{
    qDebug() << "[PROXY] New connection";
    QTcpSocket *client = qobject_cast<QTcpSocket*>(sender());
    if (!client)
    {
        qDebug() << "[PROXY] ERROR!";
        return;
    }

    m_buffers[client] += client->readAll();

    // Ожидаем конца HTTP заголовков
    auto &buf = m_buffers[client];
    int headerEnd = buf.indexOf("\r\n\r\n");
    if (headerEnd < 0)
        return; // Еще не прочитали заголовки

    QByteArray headers = buf.left(headerEnd);
    //buf.clear();

    handleRequest(client, headers);
}



// -- Разбор входящего HTTP-запроса ---------

void LocalMediaProxy::handleRequest(QTcpSocket *client, const QByteArray &rawHeaders)
{
    QByteArrayList lines = rawHeaders.split('\n');
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
    proxyToBackend2(client, method, fileId, lines);
}

void LocalMediaProxy::proxyToBackend2(QTcpSocket *clientSocket, const QString &method, const QString &fileId,  QByteArrayList &lines)
{
    QTcpSocket *serverSocket = new QTcpSocket(this);
    lines = normalizeLineEndings(lines);
    QString token;
    {
        QMutexLocker lock(&m_tokenMutex);
        token = m_token;
    }
    QString url = "/api/files/" + fileId + "/download";
    QString header1 = QString(method + " " + url + " " + "HTTP/1.1" + "\r\n");

    lines[0] = header1.toLatin1();

    for (auto &line : lines)
    {
        if (line.startsWith("Host"))
        {
            line = "Host: 127.0.0.1:8080\r\n";
        }
    }

    QString headerToken = "Authorization: Bearer " + m_token + "\r\n";
    lines.append(headerToken.toLatin1());

    lines.append("\r\n");
    // Отправка запроса с клиента на сервер
    connect(serverSocket, &QTcpSocket::connected, this, [clientSocket, serverSocket, lines, this] ()
    {
        if (!serverSocket)
            return;
        if (!clientSocket)
            return;

        if (m_buffers.contains(clientSocket))
        {
            qDebug() << "Отправлено на сервер" << serverSocket->write(lines.join());
            qDebug() <<  lines.join().size() << lines.join();
            m_buffers.remove(clientSocket);
        }
    });

    // Отправка ответа с сервера на клиент
    connect(serverSocket, &QTcpSocket::readyRead, this, [clientSocket, serverSocket, this]
    {
        if (!serverSocket)
            return;
        if (!clientSocket)
            return;
        auto responseData = serverSocket->readAll();
        qDebug() << "[PROXY] Получено данных с сервера:" << responseData.size();
        int headerEnd = responseData.indexOf("\r\n\r\n");
        if (headerEnd != -1)
        {
            //qDebug() << "[PROXY] Заголовки ответа сервера:" << responseData.left(headerEnd);
        }

        clientSocket->write(responseData);

    });

    connect(serverSocket, &QTcpSocket::disconnected, this, &LocalMediaProxy::onDisconnected);


    m_clientToServer.insert(clientSocket, serverSocket);
    m_serverToClient.insert(serverSocket, clientSocket);
    serverSocket->connectToHost("127.0.0.1", 8080);
}

void LocalMediaProxy::onDisconnected()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());
    if (!socket) return;

    if (m_clientToServer.contains(socket))
    {
        QTcpSocket *serverSocket = m_clientToServer.take(socket);

        //Если отключился клиент - разрыв соединения с сервером
        if (serverSocket)
        {
            m_serverToClient.remove(serverSocket);
            serverSocket->disconnectFromHost();
            serverSocket->deleteLater();
        }
        else if (m_serverToClient.contains(socket))
        {
            auto *clientSocket = m_serverToClient.take(socket);
            if (clientSocket)
            {
                m_clientToServer.remove(clientSocket);
                clientSocket->disconnectFromHost();
                clientSocket->deleteLater();
            }
        }
    }
}

void LocalMediaProxy::proxyToBackend(QTcpSocket *client, const QString &method, const QString &fileId, QByteArrayList &lines)
{
    //lines = normalizeLineEndings(lines);
    QString token;
    {
        QMutexLocker lock(&m_tokenMutex);
        token = m_token;
    }
    QUrl url = m_backendUrl + "/api/files/" + fileId + "/download";
    QNetworkRequest req(url);
    req.setAttribute(QNetworkRequest::CacheLoadControlAttribute, QNetworkRequest::AlwaysNetwork);

    // Запись заголовков от фронта в новый запрос
    for (const auto &line : lines)
    {
        if (line.startsWith("GET") || line.startsWith("HEAD"))
        {
            continue;
        }

        QList<QByteArray> splitLine = line.split(' ');
        if (splitLine.size() > 2)
        {
            qDebug() << "[PROXY] Ошибка парсинга строки заголовка запроса";
            return;
        }
        req.setRawHeader(splitLine.first(), splitLine.at(1));
    }
    // Устновка заголовка авторизации
    req.setRawHeader("Authorization", QString("Bearer " + token).toUtf8());

    QNetworkReply *reply = (method == "HEAD") ? m_nam->head(req) : m_nam->get(req);

    // Пересылка ответа сервера на клиент
    connect(reply, &QNetworkReply::finished, this, [this, client, reply] ()
            {
                if (!client)
                {
                    return;
                }

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
                    //            if (passthroughHeaders.contains(name))
                    //            {
                    //                resp += hdr.first + ": " + hdr.second + "\r\n";
                    //            }

                    resp += hdr.first + ": " + hdr.second + "\r\n";
                }

                //resp += "Connection: close\r\n";
                resp += "\r\n";

                auto receivedData = reply->readAll();
                qDebug() << "[PROXY] Получены данные:" << receivedData.size() << "байт";
                qDebug() << "[PROXY] Полученный заголовок ответа:" << resp;
                resp += std::move(receivedData);

                client->write(resp);
                client->flush();
                client->disconnectFromHost();
                reply->deleteLater();
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

