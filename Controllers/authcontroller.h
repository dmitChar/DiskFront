#ifndef AUTHCONTROLLER_H
#define AUTHCONTROLLER_H

#include <QObject>
#include <QSettings>

#include "apiservice.h"
#include "models/usermodel.h"

class AuthController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool loggedIn READ loggedIn NOTIFY authStateChanged)
    Q_PROPERTY(bool busy READ busy NOTIFY busyChanged)
    Q_PROPERTY(QString error READ error NOTIFY errorChanged)

public:
    explicit AuthController(APIService *api, UserModel *model, QObject *parent = nullptr);

    bool loggedIn() const { return m_loggedIn; }
    bool busy()     const { return m_busy; }
    QString error() const { return m_error; }

    Q_INVOKABLE void login(const QString &login, const QString &password);
    Q_INVOKABLE void registerUser(const QString &login, const QString &email, const QString password);
    Q_INVOKABLE void logout();
    Q_INVOKABLE void loadSavedSession();
    Q_INVOKABLE void clearError();

private:
    APIService  *m_api = nullptr;
    UserModel   *m_user = nullptr;
    QSettings   m_settings;
    bool        m_loggedIn = false;
    bool        m_busy     = false;
    QString     m_error;

private:
    void setBusy(bool v);
    void setError(const QString &err);
    void applyToken(const QString &token, const QJsonObject &userData);


signals:
    void authStateChanged();
    void busyChanged();
    void errorChanged();

    void loginSuccess();
    void registerSuccess();
    void logoutSuccess();

};

#endif // AUTHCONTROLLER_H
