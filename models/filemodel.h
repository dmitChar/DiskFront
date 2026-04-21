#ifndef FILEMODEL_H
#define FILEMODEL_H

#include <QObject>
#include <QAbstractListModel>
#include <Utils/fileitem.h>




class FileModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(QString currentPath READ currentPath NOTIFY currentPathChanged)
    Q_PROPERTY(int count READ count NOTIFY countChanged)
    Q_PROPERTY(bool loading READ loading NOTIFY loadingChanged)
    Q_PROPERTY(SortType sortType READ sortType WRITE setSortType NOTIFY sortTypeChanged)
public:
    enum Roles
    {
        IdRole = Qt::UserRole + 1,  // id файла
        NameRole,                   // Название файла
        PathRole,                   // Путь к файлу(на клиенте)
        TypeRole,                   // Тип файла
        SizeBytesRole,              // Размер файла
        HumanSizeRole,
        MimeTypeRole,
        SUffixTypeRole,
        IsSharedRole,
        ShareTokenRole,
        IconNameRole,
        CreatedAtRole,
        UpdatedAtRole,
        IsDirRole
    };

    enum SortType
    {
        NameSortUp = 1, // По возрастанию букв(Сначала а,б,в...)
        NameSortDown,   // По убыванию букв(Сначала я,ю,э...)
        SizeSortUp,     // По возрастанию размера(Сначала маленькие)
        SizeSortDown,   // По убыванию размера(Сначала большие)
        DataSortUp,     // По возрастанию даты изменения(сначала новые)
        DataSortDown    // По убыванию даты(сначала старые)
    };
    Q_ENUM(SortType)

    explicit FileModel(QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    QString currentPath() const {return m_currentPath; }
    bool loading() const {return m_loading; }
    int count() const { return m_items.size(); }
    SortType sortType() { return m_sortType; }
    void setSortType(SortType type) { m_sortType = type; }

     void setFiles(const QJsonArray &arr, const QString &path);
     void setLoading(bool v);
     Q_INVOKABLE void clear();
     Q_INVOKABLE FileItem fileAt(int index) const;
     Q_INVOKABLE int indexOf(const QString &path) const;
     QString sort(SortType type);


private:
    QList<FileItem> m_items;
    QString m_currentPath;
    bool m_loading = false;
    SortType m_sortType = NameSortUp;



signals:
    void currentPathChanged();
    void countChanged();
    void sortChanged();
    void loadingChanged();
    void sortTypeChanged();
};

#endif // FILEMODEL_H
