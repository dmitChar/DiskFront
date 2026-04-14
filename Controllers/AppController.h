#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include "apiservice.h"
#include "Controllers/authcontroller.h"
#include "Controllers/filecontroller.h"
#include "models/filemodel.h"
#include "models/usermodel.h"
#include "models/filemodel.h"
#include "models/transfermodel.h"

class AppController : public QObject
{
    Q_OBJECT
public:
    explicit AppController(QObject *parent = nullptr);

    APIService* getApi() {return m_apiService;}
    AuthController* getAuth() { return m_authController; }
    FileController *getFileCtrl() {return m_fileController; }
    UserModel *getUserModel() {return m_userModel; }

private:
    APIService *m_apiService;
    AuthController *m_authController;
    FileController *m_fileController;

    UserModel *m_userModel;
    FileModel *m_fileModel;
    TransferModel *m_transModel;


signals:

public slots:
};

#endif // CONTROLLER_H
