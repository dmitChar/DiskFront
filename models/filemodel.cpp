#include "filemodel.h"
#include <algorithm>

FileModel::FileModel(QObject *parent)
    :QAbstractListModel(parent)
{

}

int FileModel::rowCount(const QModelIndex &parent) const
{
    return m_items.size();
}

QVariant FileModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_items.size())
        return {};
    const FileItem &f = m_items[index.row()];

    switch (role)
    {
        case IdRole: return f.id;
        case NameRole: return f.name;
        case PathRole: return f.path;
        case TypeRole: return f.type;
        case SizeBytesRole: return f.sizeBytes;
        case HumanSizeRole: return f.humanSize();
        case MimeTypeRole: return f.mimeType;
        case SUffixTypeRole: return f.suffix;
        case IsSharedRole: return f.isShared;
        case ShareTokenRole: return f.shareToken;
        case IconNameRole: return f.iconName();
        case IsDirRole: return f.isDir();
        case CreatedAtRole: return f.createdAt.toString("dd.MM.yyyy");
        case UpdatedAtRole: return f.updatedAt.toString("dd.MM.yyyy HH:mm");
        default: return {};
    }
}


QHash<int, QByteArray> FileModel::roleNames() const
{
    return
    {
        {IdRole, "fileId"},
        {NameRole, "fileName"},
        {PathRole, "filePath"},
        {TypeRole, "fileType"},
        {SizeBytesRole, "sizeBytes"},
        {HumanSizeRole, "humanSize"},
        {MimeTypeRole, "mimeType"},
        {SUffixTypeRole, "suffixType"},
        {IsSharedRole, "isShared"},
        {ShareTokenRole, "shareToken"},
        {IconNameRole, "iconName"},
        {IsDirRole, "isDir"},
        {CreatedAtRole, "createdAt"},
        {UpdatedAtRole, "updatedAt"}
    };
}

void FileModel::setFiles(const QJsonArray &arr, const QString &path)
{
    beginResetModel();
    m_items.clear();
    for (const auto &v: arr)
    {
        if (v.isObject())
            m_items.append(FileItem::fromJson(v.toObject()));
    }
    m_currentPath = path;
    sort(m_sortType);
    endResetModel();
    emit currentPathChanged();
    emit countChanged();
}

void FileModel::setLoading(bool v)
{
    if (m_loading == v) return;
    m_loading = v;
    emit loadingChanged();
}

void FileModel::clear()
{
    beginResetModel();
    m_items.clear();
    endResetModel();
    emit countChanged();
}

/**
 * @brief FileModel::fileAt Функция, возвращающая файл из модели согласно заданному индексу
 * @param index
 * @return
 */
FileItem FileModel::fileAt(int index) const
{
    if (index < 0 || index >= m_items.size())
        return {};
    return m_items[index];
}

/**
 * @brief FileModel::indexOf Функция для поиска индекса в модели по заданному пути
 * @param path Путь, по которому осуществляется поиск
 * @return
 */
int FileModel::indexOf(const QString &path) const
{
    for (int i = 0; i < m_items.size(); ++i)
    {
        if (m_items[i].path == path) return i;
    }
    return -1;
}

QString FileModel::sort(SortType type)
{
    m_sortType = type;
    beginResetModel();
    QString error = "";

    switch (m_sortType)
    {
    case NameSortUp:
        std::sort(m_items.begin(), m_items.end(), [] (const FileItem &item1, const FileItem &item2)
        {
            return item1.name < item2.name;
        });
        break;

    case NameSortDown:
        std::sort(m_items.begin(), m_items.end(), [] (const FileItem &item1, const FileItem &item2)
        {
            return item1.name > item2.name;
        });
        break;

    case SizeSortUp:
        std::sort(m_items.begin(), m_items.end(), [] (const FileItem &item1, const FileItem &item2)
        {
             return item1.sizeBytes < item2.sizeBytes;
        });
        break;

    case SizeSortDown:
        std::sort(m_items.begin(), m_items.end(), [] (const FileItem &item1, const FileItem &item2)
        {
            return item1.sizeBytes > item2.sizeBytes;
        });
        break;

    case DataSortUp:
        std::sort(m_items.begin(), m_items.end(), [] (const FileItem &item1, const FileItem &item2)
        {
            return item1.updatedAt < item2.updatedAt;
        });
        break;

    case DataSortDown:
        std::sort(m_items.begin(), m_items.end(), [] (const FileItem &item1, const FileItem &item2)
        {
            return item1.updatedAt > item2.updatedAt;
        });
        break;

    default:
        error = "Unknown sort error";
        break;
    }

    endResetModel();
    return error;
}
