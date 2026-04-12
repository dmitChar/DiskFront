#include "authcontroller.h"

AuthController::AuthController(APIService *api, UserModel *model, QObject *parent)
    : QObject(parent), m_api(api), m_user(model), m_settings("CloudDisk", "Client")
{

}

void AuthController::setBusy(bool v)
{
    if (m_busy == v) return;
    m_busy = v;
    emit busyChanged();
}

void AuthController::setError(const QString &err)
{
    m_error = err;
    emit errorChanged();
}

// Установка нового JWT токена
void AuthController::applyToken(const QString &token, const QJsonObject &userData)
{
    m_api->setToken(token);
    m_settings.setValue("auth/token", token);
    m_user->setUserFromJson(userData);
    m_loggedIn = true;
    emit authStateChanged();
}

//
void AuthController::login(const QString &login, const QString &password)
{
    if (login.isEmpty() || password.isEmpty())
    {
        setError("Заполните все поля");
        return;
    }
    setBusy(true);
    setError({});

    m_api->postLogin(login, password, [this] (ApiResponse r)
    {
        setBusy(false);
        if (!r.succes)
        {
            setError(r.errorMsg);
            return;
        }
        QString token = r.data["token"].toString();
        QJsonObject user = r.data["user"].toObject();
        applyToken(token, user);
        emit authStateChanged();
        emit loginSuccess();
    });
}

void AuthController::registerUser(const QString &login, const QString &email, const QString password)
{
    setBusy(true);
    setError({});

    m_api->postRegister(login, email, password, [this] (ApiResponse r)
    {
        setBusy(false);
        if (!r.succes)
        {
            setError(r.errorMsg);
            return;
        }
        emit registerSuccess();
    });
}

void AuthController::logout()
{

}

void AuthController::loadSavedSession()
{
    QString token = m_settings.value("auth/token").toString();
    if (token.isEmpty()) return;

    m_api->setToken(token);
    setBusy(true);

    m_api->getMe([this] (ApiResponse r)
    {
        setBusy(false);

        if (r.succes)
        {
            m_user->setUserFromJson(r.data);
            m_loggedIn = true;

            emit authStateChanged();
            emit loginSuccess();
        }
        else // Токен устарел
        {
            m_settings.remove("auth/toke");
            m_api->setToken({});
        }
    });
}

void AuthController::clearError()
{
    setError({});
}
