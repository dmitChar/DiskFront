#include "AppController.h"

AppController::AppController(QObject *parent)
    : QObject(parent)
{
    m_apiService = new APIService(this);
    m_userModel = new UserModel(this);
    m_fileModel = new FileModel(this);
    m_transModel = new TransferModel(this);
    m_userModel = new UserModel(this);

    m_authController = new AuthController(m_apiService, m_userModel, this);
    m_fileController = new FileController(m_apiService, m_fileModel, m_transModel, this);

    m_apiService->setBaseUrl("http://127.0.0.1:8080");
    m_authController->loadSavedSession();

    connect(m_fileController, &FileController::updateQuota, this, &AppController::refreshQuota);
}

void AppController::refreshQuota()
{
    m_fileController->setBusy(true);
    m_fileController->setError({});
    m_apiService->getUserQuota([this] (ApiResponse r)
    {
        if (r.succes)
            m_userModel->setQuotaFromJson(r.data);
        else
            m_fileController->setError(r.errorMsg);
        m_fileController->setBusy(false);

        return;
    });
}

