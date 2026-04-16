#include "filecontroller.h"
#include <QFile>
#include <QFileInfo>

FileController::FileController(APIService *api, FileModel *model, TransferModel *transfers, QObject *parent)
    : m_api(api), m_model(model), m_transfers(transfers), QObject(parent)
{

}

QStringList FileController::breadcrumbs() const
{
    if (m_currentPath == "/" || m_currentPath.isEmpty())
        return {"/"};
    QStringList parts = m_currentPath.split("/");
    QStringList crumbs;
    crumbs << "/";

    for (const auto &part : parts)
        crumbs.append(part);
    return crumbs;
}

void FileController::navigateTo(const QString &path)
{
    loadDir(path);
}

void FileController::navigateUp()
{
    if (m_currentPath == "/" || m_currentPath.isEmpty()) return;
    QString parent = m_currentPath.section('/', 0, 2);

    if (parent.isEmpty())
        parent = "/";
    loadDir(parent);
}

void FileController::refresh()
{
    loadDir(m_currentPath);
    emit updateQuota();
}

void FileController::mkCopy(const QString &filename)
{
    QString path = (m_currentPath.endsWith('/') ? m_currentPath : m_currentPath + "/") + filename.trimmed();
    setBusy(true);
    setError({});
    m_api->postCopy(path + filename, path + filename + "(1)", [this] (ApiResponse r)
    {
        setBusy(false);
        if (!r.succes)
        {
            setError(r.errorMsg);
            return;
        }
        refresh();
    });
}

void FileController::moveFile(const QString &from, const QString &to)
{
    setBusy(true);
    setError({});
    m_api->postMove(from, to, [this] (ApiResponse r)
    {
        setBusy(false);
        if (!r.succes)
        {
            setError(r.errorMsg);
            return;
        }
        refresh();
    });
}

void FileController::deleteItem(const QString &path)
{
    setBusy(true);
    setError({});
    m_api->deleteItem(path, [this, path] (ApiResponse r)
    {
        setBusy(false);
        if (!r.succes)
        {
            setError(r.errorMsg);
            return;
        }
        emit itemDeleted(path);
        refresh();
    });
}

void FileController::mkdir(const QString &folderName)
{
    if (folderName.trimmed().isEmpty())
    {
        setError("Введите название папки");
        return;
    }
    QString path = (m_currentPath.endsWith('/') ? m_currentPath : m_currentPath + "/") + folderName.trimmed();
    setBusy(true);
    setError({});
    m_api->postMkdir(path, [this] (ApiResponse r)
    {
        setBusy(false);
        if (!r.succes)
        {
            setError(r.errorMsg);
            return;
        }
        refresh();
    });
}

/**
 * @brief FileController::uploadFiles Функция для отправки файлов на сервер
 * @param localPaths Список путей файлов, которые необходимо отправить
 */
void FileController::uploadFiles(const QList<QUrl> &localPaths)
{
    for (const QUrl &p : localPaths)
    {
        QString path = p.toLocalFile();
        path.remove("file://");
        //path.replace("file://", "");
        qDebug() << "Путь:" << path;
        QFile f(path);
        if (!f.open(QIODevice::ReadOnly))
            continue;
        QByteArray data = f.readAll();
        QString filename = QFileInfo(path).fileName();

        auto *reply = m_api->uploadFile(m_currentPath, path, filename, data);
        int transferId = m_transfers->addUpload(filename, m_currentPath + "/" + filename, reply);
        Q_UNUSED(transferId)

        connect(reply, &QNetworkReply::finished, this, [this] ()
        {
            refresh();
        });
    }
}

void FileController::downloadFile(const QString &remotePath, const QString &localPath)
{
    QString name = remotePath.section('/', -1);
    auto *reply = m_api->downloadFile(remotePath);
    m_transfers->addDownload(name, remotePath, localPath, reply);
}

void FileController::clearError()
{
    setError({});
}

void FileController::setBusy(bool v)
{
    m_busy = v;
    emit busyChanged();
}

void FileController::setError(const QString &error)
{
    m_error = error;
    emit errorChanged();
}

void FileController::loadDir(const QString &path)
{
    setBusy(true);
    setError({});
    m_model->setLoading(true);

    m_api->getFiles(path, [this, path] (ApiResponse r)
    {
        setBusy(false);
        m_model->setLoading(false);
        if (!r.succes)
        {
            setError(r.errorMsg);
            return;
        }
        m_currentPath = path.isEmpty() ? "/" : path;
        m_model->setFiles(r.dataArray, m_currentPath);
        emit pathChanged();
    });
}
