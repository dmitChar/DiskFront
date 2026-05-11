#include "filecontroller.h"
#include <QFile>
#include <QFileInfo>

FileController::FileController(APIService *api, FileModel *model, TransferModel *transfers, ProxyModel *proxy, UserModel *userModel, QObject *parent)
    : m_api(api), m_model(model), m_transfers(transfers), m_proxy(proxy), m_userModel(userModel), QObject(parent)
{
}

QStringList FileController::breadcrumbs() const
{
    if (m_currentPath == "/" || m_currentPath.isEmpty())
        return {"/"};
    QStringList parts = m_currentPath.split("/", Qt::SkipEmptyParts);
    QStringList crumbs;
    crumbs << "/";

    for (const auto &part : parts)
        crumbs << part;
    return crumbs;
}

void FileController::navigateTo(const QString &path)
{
    loadDir(path);
}

void FileController::navigateUp()
{
    if (m_currentPath == "/" || m_currentPath.isEmpty())
        return;
    QStringList strList = m_currentPath.split("/");
    strList.removeLast();

    QString parent = "/";
    if (!strList.isEmpty())
        parent += strList.join("/");

    //QString parent = m_currentPath.section('/', 0, 2);

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

void FileController::deleteItem(int index)
{
    FileItem file = m_model->fileAt(proxyToSourceIndex(index));
    QString path = file.path;
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

void FileController::renameFile(int index, const QString &newName)
{
    FileItem file = m_model->fileAt(proxyToSourceIndex(index));
    QString path = file.path;
    setBusy(true);
    setError({});
    QJsonObject obj;
    obj["newName"] = newName;

    m_api->postRenameFile(path, obj, [this, path] (ApiResponse r)
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

void FileController::downloadFile(qint64 fileID)
{
    FileItem file = m_model->fileAt(proxyToSourceIndex(fileID));

    auto reply = m_api->downloadFile(file.id, m_userModel->userId());
    if (reply)
        m_transfers->addDownload(file.name, m_currentPath + "/" + file.name, reply.value());

    //connect(reply, APIService::fileReady, this);

    //    QString name = remotePath.section('/', -1);
    //    auto *reply = m_api->downloadFile(remotePath);
    //    m_transfers->addDownload(name, remotePath, localPath, reply);
}

void FileController::clearError()
{
    setError({});
}

void FileController::sort(int type)
{
    setBusy(true);
    QString m_error = m_model->sort(static_cast<FileModel::SortType>(type));
    if (m_error != "")
        setError(m_error);
    setBusy(false);
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

QString FileController::getFileType(int index)
{
    return m_model->fileAt(proxyToSourceIndex(index)).getIconName();
}

bool FileController::isDir(int index)
{
    return m_model->fileAt(proxyToSourceIndex(index)).isDir();
}

QString FileController::getFilePath(int index)
{
    return m_model->fileAt(proxyToSourceIndex(index)).path;
}

int FileController::proxyToSourceIndex(int proxyIndex)
{
    if (proxyIndex < 0)
        return -1;
    QModelIndex proxyIdx = m_proxy->index(proxyIndex, 0);
    QModelIndex sourceIdx = m_proxy->mapToSource(proxyIdx);
    return sourceIdx.row();
}
