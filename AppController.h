#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <apiservice.h>

class AppController : public QObject
{
    Q_OBJECT
public:
    explicit AppController(QObject *parent = nullptr);
    Q_INVOKABLE void sendAuthenticate(const QString &login, const QString &passwd);

private:
    APIService *m_apiService;
signals:

public slots:
};

#endif // CONTROLLER_H
