#include "AppController.h"

AppController::AppController(QObject *parent)
    : QObject(parent)
{
    m_apiService = new APIService(this);
    m_apiService->setBaseUrl("http://127.0.0.1:8080");
    m_userModel = new UserModel(this);
    m_fileModel = new FileModel(this);
    m_transModel = new TransferModel(this);
    m_userModel = new UserModel(this);
    m_proxyModel = new ProxyModel(this);


    m_authController = new AuthController(m_apiService, m_userModel, m_transModel, this);
    m_fileController = new FileController(m_apiService, m_fileModel, m_transModel, m_proxyModel, m_userModel, this);


    m_authController->loadSavedSession();
    m_proxyModel->setSourceModel(m_fileModel);
    m_proxyModel->invalidate();

    m_media = new MediaController(m_apiService, m_fileController, m_transModel, this);

    connect(m_fileController, &FileController::updateQuota, this, &AppController::refreshQuota);
    connect(m_authController, &AuthController::loginSuccess, this, [this] ()
    {
        m_fileController->navigateTo("/");
        m_fileController->refresh();
        m_media->onLogin(m_apiService->getJwtToken(), m_userModel->userId());
    });
    connect(m_authController, &AuthController::logoutSuccess, this, [this] ()
    {
        m_media->onLogout();
    });
}

void AppController::registerMediaProvider(QQmlEngine *engine)
{
    m_media->registerImageProvider(engine);
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

void AppController::setSearchText(const QString &text)
{
    m_proxyModel->setSearch(text);
}

