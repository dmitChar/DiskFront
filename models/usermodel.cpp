#include "usermodel.h"
#include <QJsonObject>
#include <QVariant>

UserModel::UserModel(QObject *parent) : QObject(parent)
{

}

void UserModel::setUserFromJson(QJsonObject obj)
{
    m_userId = obj["id"].toVariant().toLongLong();
}

QString UserModel::humanize(qint64 bytes)
{
    if (bytes < 1024) return QString("%1 B").arg(bytes);
    if (bytes < 1024 * 1024) return QString("%1 KB").arg(bytes / 1024.0, 0, 'f', 1);
    if (bytes < 1024LL *1024 * 1024) return QString("%1 MB").arg(bytes / 1024.0 / 1024.0, 0, 'f', 1);

    return QString ("%1 GB").arg(bytes / 1024.0 / 1024.0 / 1024.0, 0, 'f', 2);
}
