#ifndef TRANSFERMODEL_H
#define TRANSFERMODEL_H

#include <QObject>
#include <QNetworkReply>
#include <QAbstractListModel>
#include <QSortFilterProxyModel>

enum class TransferState
{
    Pending,
    Active,
    Done,
    Failed
};

struct Transfer
{
    int id;
    QString name;
    QString path;       //server path
    QString localPath;  //local path for downloads
    bool isUpload;
    TransferState state = TransferState::Pending;
    qint64 bytesTotal = 0;
    qint64 bytesDone = 0;
    QString errorMsg;
    QNetworkReply *reply = nullptr;

    int progress() const
    {
        if (bytesTotal <= 0) return 0;
        return int(bytesDone *100 / bytesTotal);
    }

    QString stateStr() const
    {
        switch (state)
        {
            case TransferState::Pending: return "pending";
            case TransferState::Active:  return "active";
            case TransferState::Done:    return "done";
            case TransferState::Failed:  return "failed";
        }
        return {};
    }
};

class TransferModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int activeCount READ activeCount  NOTIFY activeCountChanged)
public:
    explicit TransferModel(QObject *parent = nullptr);

    enum Roles
    {
        IdRole = Qt::UserRole +1,
        NameRole,
        PathRole,
        IsUploadRole,
        StateRole,
        ProgressRole,
        ErrorRole
    };

    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    // Функции
    int addUpload(const QString &name, const QString &remotePath, QNetworkReply *reply);
    int addDownload(const QString &name, const QString &remotePath, const QString &localPath, QNetworkReply *reply);

    Q_INVOKABLE int activeCount() const;
    Q_INVOKABLE void clearCompleted();


private:
    int indexById(int id) const;
    void connectReply(int id, QNetworkReply *reply, bool isUpload);
    void updateProgress(int id, qint64 done, qint64 total);

    void setFailed(int id, const QString &errorMsg);
    void setDone(int id);


    QVector<Transfer> m_transfers;
    int m_nextId = 1;       // Id для трансферов

signals:
    void activeCountChanged();
    void transferCompleted(int id, bool success, const QString &name);

public slots:
};

#endif // TRANSFERMODEL_H
