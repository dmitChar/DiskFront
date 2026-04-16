#include "AppController.h"

AppController::AppController(QObject *parent) : QObject(parent), m_apiService(new APIService(this))
{
    m_apiService->setBaseUrl("http://localhost:8080");
}

void AppController::sendAuthenticate(const QString &login, const QString &passwd)
{
    m_apiService->postAuth(login, passwd, [](ApiResponse r)
    {

    });
}
