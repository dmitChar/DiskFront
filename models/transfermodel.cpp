#include "transfermodel.h"
#include <QFile>

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

void TransferModel::clearCompleted()
{
    beginResetModel();
    m_transfers.erase(
    std::remove_if(m_transfers.begin(), m_transfers.end(), [] (const Transfer &t)
    {
        return t.state == TransferState::Done || t.state == TransferState::Failed;
    }), m_transfers.end());

    endResetModel();
    emit activeCountChanged();
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
    int id = m_nextId++;
    Transfer t;
    t.id = id; t.name = name;
    t.path = remotePath; t.isUpload = true;
    t.state = TransferState::Active; t.reply = reply;

    beginInsertRows({}, m_transfers.size(), m_transfers.size());
    m_transfers.append(t);
    endInsertRows();

    connectReply(id, reply, true);
    emit activeCountChanged();
    return id;
}

int TransferModel::addDownload(const QString &name, const QString &remotePath, const QString &localPath, QNetworkReply *reply)
{
    int id = m_nextId++;
    Transfer t;
    t.id = id; t.name = name;
    t.path = remotePath; t.localPath = localPath;
    t.isUpload = false; t.state = TransferState::Active; t.reply = reply;

    beginInsertRows({}, 0, 0);
    m_transfers.prepend(t);
    endInsertRows();
    connectReply(id, reply, false);
    emit activeCountChanged();
    return id;
}

void TransferModel::connectReply(int id, QNetworkReply *reply, bool isUpload)
{
    auto progressSignal = isUpload ? &QNetworkReply::uploadProgress : &QNetworkReply::downloadProgress;

    connect(reply, progressSignal, this, [this, id] (qint64 done, qint64 total)
    {
        updateProgress(id, done, total);
    });

    connect(reply, &QNetworkReply::finished, this, [this, id, reply, isUpload]
    {
        if (reply->error() != QNetworkReply::NoError)
        {
            setFailed(id, reply->errorString());
        }
        else
        {
            int idx = indexById(id);
            if (!isUpload && idx >= 0)
            {
                // Сохранение загруженного файла
                const QString &localPath = m_transfers[idx].localPath;
                if (!localPath.isEmpty())
                {
                    QFile f(localPath);
                    if (f.open(QIODevice::WriteOnly))
                    {
                        f.write(reply->readAll());
                        f.close();
                    }
                }
            }
            setDone(id);
        }
        reply->deleteLater();
    });
}

/**
 * @brief TransferModel::updateProgress Функция обновляющая количество загруженных байт в модели для указанного трансфера
 * @param id Номер трансфера, для которого обновляются данные
 * @param done Количество скачанных/загруженных байт
 * @param total Общий размер файла в байтах
 */
void TransferModel::updateProgress(int id, qint64 done, qint64 total)
{
    if (done == 0) return;
    int idx = indexById(id);
    if (idx < 0) return;
    m_transfers[idx].bytesDone = done;
    m_transfers[idx].bytesTotal = total;
    auto mi = index(idx);
    emit dataChanged(mi, mi, {ProgressRole});
}

/**
 * @brief TransferModel::setFailed Функция, уведомляющая о том, что загрузка файла была завершена аварийно
 * @param id Номер трансфера, для которого не удлалась загрузка
 * @param errorMsg Сообщение об ошибке от reply
 */
void TransferModel::setFailed(int id, const QString &errorMsg)
{
    int idx = indexById(id);
    if (idx < 0) return;
    m_transfers[idx].state = TransferState::Failed;
    m_transfers[idx].errorMsg = errorMsg;

    auto mi = index(idx);
    emit dataChanged(mi, mi, {StateRole, ErrorRole});
    emit transferCompleted(id, false, m_transfers[idx].name);
    emit activeCountChanged();
}

// Функция, уведомляющая о том, что завершена загузка файла с id
// id - Transfer.id
// idx - индекс в модели QVector<Transfer> m_transfers
// mi - индекс в отображаемой модели
void TransferModel::setDone(int id)
{
    int idx = indexById(id);
    if (idx < 0) return;
    m_transfers[idx].state = TransferState::Done;
    m_transfers[idx].bytesDone = m_transfers[idx].bytesTotal;

    auto mi = index(idx);
    emit dataChanged(mi, mi, {StateRole, ProgressRole});    // Обновление данных в отбражении
    emit transferCompleted(id, true, m_transfers[idx].name);
    emit activeCountChanged();
}

