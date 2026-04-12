#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include "apiservice.h"
#include "Controllers/authcontroller.h"
#include "models/filemodel.h"
#include "models/usermodel.h"

class AppController : public QObject
{
    Q_OBJECT
public:
    explicit AppController(QObject *parent = nullptr);

    APIService* getApi() {return m_apiService;}
    AuthController* getAuth() { return m_authController; }

private:
    APIService *m_apiService;
    AuthController *m_authController;
    UserModel *m_userModel;


signals:

public slots:
};

#endif // CONTROLLER_H
