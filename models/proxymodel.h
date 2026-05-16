#ifndef PROXYMODEL_H
#define PROXYMODEL_H

#include <QObject>
#include <QSortFilterProxyModel>
#include <models/filemodel.h>

class ProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
    Q_PROPERTY(QString search READ search WRITE setSearch NOTIFY searchChanged)
public:
    explicit ProxyModel(QObject* parent = nullptr);

    QString search() const { return m_search; }
    void setSearch(const QString &s);

protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;

private:
    QString m_search;

signals:
    void searchChanged();

};

#endif // PROXYMODEL_H
