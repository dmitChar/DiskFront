#ifndef LOCALMEDIAPROXY_H
#define LOCALMEDIAPROXY_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QMutex>
#include <QMap>
#include <QByteArrayList>

class LocalMediaProxy : public QObject
{
    Q_OBJECT
    Q_PROPERTY(quint16 port READ port CONSTANT)

public:
    explicit LocalMediaProxy(const QString &backendUrl, QObject *parent = nullptr);
    ~LocalMediaProxy();

    bool start();
    bool stop();
    quint16 port() const {return m_port;}
    bool isRunning() const { return m_server->isListening(); }

    Q_INVOKABLE QString mediaUrl(const QString &fileId) const;

    void setToken(const QString &newToken);
    void setUserId(qint64 newUserId);

private slots:
    void onNewConnection();
    void onClientReadyRead();
    void onDisconnected();

private:
    void handleRequest(QTcpSocket *client, const QByteArray &rawHeaders);
    void sendError(QTcpSocket *client, int code, const QString &msg);

    void proxyToBackend(QTcpSocket *client, const QString &method, const QString &fileId, QByteArrayList &lines);
    void proxyToBackend2(QTcpSocket *client, const QString &method, const QString &fileId, QByteArrayList &lines);

    QByteArray buildStatusLine(int code);

private:
    struct ClientState
    {
        QByteArray buffer;
        bool headersDone = false;
    };

    QTcpServer *m_server;
    quint16 m_port;

    QString m_backendUrl;
    QString m_token;
    qint64 m_userId = 0;
    QMutex m_tokenMutex;

    // Для каждого клиентсокого сокета - свое состояние
    QMap<QTcpSocket*, QByteArray> m_buffers;
    QMap<QTcpSocket*, QTcpSocket*> m_clientToServer;
    QMap<QTcpSocket*, QTcpSocket*> m_serverToClient;

    QNetworkAccessManager *m_nam;


};

#endif // LOCALMEDIAPROXY_H
