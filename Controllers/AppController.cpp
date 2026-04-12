#include "AppController.h"

AppController::AppController(QObject *parent)
    : QObject(parent)
{
    m_apiService = new APIService(this);
    m_userModel = new UserModel(this);
    m_authController = new AuthController(m_apiService, m_userModel, this);

    m_apiService->setBaseUrl("http://127.0.0.1:8080");
}

