#include "usermodel.h"
#include <QJsonObject>
#include <QVariant>

UserModel::UserModel(QObject *parent) : QObject(parent)
{

}

void UserModel::setUserFromJson(const QJsonObject &obj)
{
    m_userId   = obj["id"].toVariant().toLongLong();
    m_username = obj["username"].toString();
    m_email    = obj["email"].toString();
    m_quotaBytes = obj["quotaBytes"].toVariant().toLongLong();
    m_usedBytes  = obj["usedBytes"].toVariant().toLongLong();
    emit userChanged();
    emit quotaChanged();
}

void UserModel::setQuotaFromJson(const QJsonObject &obj)
{
    m_quotaBytes = obj["totalBytes"].toVariant().toLongLong();
    m_usedBytes  = obj["usedBytes"].toVariant().toLongLong();
    emit quotaChanged();
}

void UserModel::clear()
{
    m_userId = 0; m_username.clear(); m_email.clear();
    m_quotaBytes = 0; m_usedBytes = 0;
    emit userChanged(); emit quotaChanged();
}

QString UserModel::humanize(qint64 bytes)
{
    if (bytes < 1024) return QString("%1 B").arg(bytes);
    if (bytes < 1024 * 1024) return QString("%1 KB").arg(bytes / 1024.0, 0, 'f', 1);
    if (bytes < 1024LL *1024 * 1024) return QString("%1 MB").arg(bytes / 1024.0 / 1024.0, 0, 'f', 1);

    return QString ("%1 GB").arg(bytes / 1024.0 / 1024.0 / 1024.0, 0, 'f', 2);
}
