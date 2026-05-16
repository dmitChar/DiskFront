#ifndef FILECONTROLLER_H
#define FILECONTROLLER_H

#include <QObject>
#include <QStringList>

#include "apiservice.h"
#include "models/filemodel.h"
#include "models/transfermodel.h"
#include "models/proxymodel.h"
#include "models/usermodel.h"

class FileController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString currentPath READ currentPath NOTIFY pathChanged)
    Q_PROPERTY(bool busy READ busy NOTIFY busyChanged)
    Q_PROPERTY(QString error READ error NOTIFY errorChanged)
    Q_PROPERTY(QStringList breadcrumbs READ breadcrumbs NOTIFY pathChanged)

public:
    explicit FileController(APIService *api, FileModel *model, TransferModel *transfers, ProxyModel *proxy, UserModel *userModel, QObject *parent = nullptr);

    QString currentPath() const { return m_currentPath; }
    bool busy() const { return m_busy; }
    QString error() const { return m_error; }
    QStringList breadcrumbs() const;

    FileItem getFileById(qint64 fileId);

    Q_INVOKABLE void navigateTo(const QString &path);
    Q_INVOKABLE void navigateUp();
    Q_INVOKABLE void refresh();
    Q_INVOKABLE void mkdir(const QString &folderName);
    Q_INVOKABLE void mkCopy(const QString &filename);
    Q_INVOKABLE void uploadFiles(const QList<QUrl> &localPaths) ;
    Q_INVOKABLE void downloadFile(int fileID);
    Q_INVOKABLE void moveFile(const QString &from, const QString &to);
    Q_INVOKABLE void deleteItem(int index);
    Q_INVOKABLE void renameFile(int index, const QString &newName);
    Q_INVOKABLE void clearError();
    Q_INVOKABLE void sort(int type);

    Q_INVOKABLE QString getFileType(int index);
    Q_INVOKABLE QString getMimeType(int index);
    Q_INVOKABLE bool isDir(int index);
    Q_INVOKABLE QString getFilePath(int index);
    Q_INVOKABLE int proxyToSourceIndex(int proxyIndex);
    Q_INVOKABLE QString getFileName(int index);
    Q_INVOKABLE QString getFileId(int index);


    void setBusy(bool v);
    void setError(const QString &error);

private:
    void loadDir(const QString &path);

    APIService *m_api;
    FileModel *m_model;
    UserModel *m_userModel;
    TransferModel *m_transfers;
    ProxyModel *m_proxy;

    QString m_currentPath = "/";
    bool m_busy = false;
    QString m_error = "";


signals:
    void busyChanged();
    void errorChanged();
    void uploadFinished();
    void pathChanged();
    void itemDeleted(const QString &path);
    void updateQuota();

};

#endif // FILECONTROLLER_H
