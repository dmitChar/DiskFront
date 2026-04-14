#ifndef FILECONTROLLER_H
#define FILECONTROLLER_H

#include <QObject>
#include <QStringList>

#include "apiservice.h"
#include "models/filemodel.h"
#include "models/transfermodel.h"

class FileController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString currentPath READ currentPath NOTIFY currentPathChanged)
    Q_PROPERTY(bool busy READ busy NOTIFY busyChanged)
    Q_PROPERTY(QString error READ error NOTIFY errorChanged)

public:
    explicit FileController(APIService *api, FileModel *model, TransferModel *transfers, QObject *parent = nullptr);

    QString currentPath() const { return m_currentPath; }
    bool busy() const { return m_busy; }
    QString error() const { return m_error; }

    Q_INVOKABLE void navigateTo(const QString &path);
    Q_INVOKABLE void navigateUp();
    Q_INVOKABLE void refresh();
    Q_INVOKABLE void mkdir(const QString &name);
    Q_INVOKABLE void mkCopy(const QString &filename);
    Q_INVOKABLE void uploadFiles(const QStringList &localPaths);
    Q_INVOKABLE void downloadFile(const QString &remotePath, const QString &localPath);
    Q_INVOKABLE void moveFile(const QString &from, const QString &to);
    Q_INVOKABLE void deleteItem(const QString &path);
    Q_INVOKABLE void clearError();

private:
    void setBusy(bool v);
    void setError(const QString &error);
    void loadDir(const QString &path);

    APIService *m_api;
    FileModel *m_model;
    TransferModel *m_transfers;
    QString m_currentPath = "/";
    bool m_busy = false;
    QString m_error;

signals:
    void currentPathChanged();
    void busyChanged();
    void errorChanged();
    void uploadFinished();
    void pathChanged();
    void itemDeleted(const QString &path);

};

#endif // FILECONTROLLER_H
