#ifndef FILEMODEL_H
#define FILEMODEL_H

#include <QObject>
#include <QAbstractListModel>
#include <fileinfo.h>

class FileModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum Roles
    {
        IdRole = Qt::UserRole + 1,
        TypeRole,
        NameRole,
        SizeRole,
        CreatedRole,
        IconRole
    };
    FileModel();
    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QHash<int, QByteArray> roleNames();

private:
    QList<FileInfo> m_data;
};

#endif // FILEMODEL_H
