#include "transfermodel.h"

TransferModel::TransferModel(QObject *parent) : QAbstractListModel(parent)
{

}

int TransferModel::rowCount(const QModelIndex &parent) const
{
    return m_transfers.size();
}

QVariant TransferModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_transfers.size())
        return {};
    const Transfer &t = m_transfers[index.row()];

    switch (role)
    {
        case IdRole: return t.id;
        case NameRole: return t.name;
        case PathRole: return t.path;
        case IsUploadRole: return t.isUpload;
        case StateRole: return t.stateStr();
        case ProgressRole: return t.progress();
        case ErrorRole: return t.errorMsg;
        default: return {};
    }
}

QHash<int, QByteArray> TransferModel::roleNames() const
{
    return
    {
        {IdRole, "transferId"},
        {NameRole, "transferName"},
        {PathRole, "transferPath"},
        {IsUploadRole, "isUploaded"},
        {StateRole, "transferState"},
        {ProgressRole, "transferProgress"},
        {ErrorRole, "transferError"}
    };
}

int TransferModel::activeCount() const
{
    int n = 0;
    for (const Transfer &tr : m_transfers)
        if (tr.state == TransferState::Active || tr.state == TransferState::Pending)
            ++n;
    return n;
}

int TransferModel::indexById(int id) const
{
    for (int i = 0; i < m_transfers.size(); ++i)
        if (m_transfers[i].id == id)
            return i;
    return -1;
}

int TransferModel::addUpload(const QString &name, const QString &remotePath, QNetworkReply *reply)
{

}

int TransferModel::addDownload(const QString &name, const QString &remotePath, const QString &localPath, QNetworkReply *reply)
{

}

void TransferModel::connectReply(int id, QNetworkReply *reply, bool isUpload)
{

}
