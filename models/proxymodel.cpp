#include "proxymodel.h"
#include <QDebug>

ProxyModel::ProxyModel(QObject *parent)
{
    setDynamicSortFilter(true);
}

void ProxyModel::setSearch(const QString &s)
{
    if (m_search == s)
        return;
    m_search = s;
    invalidateFilter();
    emit searchChanged();

}

bool ProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    QModelIndex index = sourceModel()->index(source_row, 0, source_parent);
    QString title = index.data(FileModel::NameRole).toString();

    if (!m_search.isEmpty() && !title.contains(m_search, Qt::CaseInsensitive))
        return false;
    return true;
}
