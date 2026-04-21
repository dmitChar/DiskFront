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
    Q_PROPERTY(QString currentPath READ currentPath NOTIFY pathChanged)
    Q_PROPERTY(bool busy READ busy NOTIFY busyChanged)
    Q_PROPERTY(QString error READ error NOTIFY errorChanged)
    Q_PROPERTY(QStringList breadcrumbs READ breadcrumbs NOTIFY pathChanged)

public:
    explicit FileController(APIService *api, FileModel *model, TransferModel *transfers, QObject *parent = nullptr);

    QString currentPath() const { return m_currentPath; }
    bool busy() const { return m_busy; }
    QString error() const { return m_error; }
    QStringList breadcrumbs() const;

    Q_INVOKABLE void navigateTo(const QString &path);
    Q_INVOKABLE void navigateUp();
    Q_INVOKABLE void refresh();
    Q_INVOKABLE void mkdir(const QString &folderName);
    Q_INVOKABLE void mkCopy(const QString &filename);
    Q_INVOKABLE void uploadFiles(const QList<QUrl> &localPaths) ;
    Q_INVOKABLE void downloadFile(const QString &remotePath, const QString &localPath);
    Q_INVOKABLE void moveFile(const QString &from, const QString &to);
    Q_INVOKABLE void deleteItem(const QString &path);
    Q_INVOKABLE void clearError();
    Q_INVOKABLE void sort(int type);

    void setBusy(bool v);
    void setError(const QString &error);

private:
    void loadDir(const QString &path);

    APIService *m_api;
    FileModel *m_model;
    TransferModel *m_transfers;
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
