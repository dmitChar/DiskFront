#ifndef FILECACHEMANAGER_H
#define FILECACHEMANAGER_H

#include <QObject>
#include <optional>
#include <QCache>
#include <QNetworkDiskCache>


using std::optional;
using std::nullopt;

class FileCacheManager : public QObject
{
    Q_OBJECT
public:
    explicit FileCacheManager(const QString &cacheDir, QObject *parent = nullptr);
    optional<QByteArray> get(const qint64 &fileId, const qint64 &userId) const;
    void put(const qint64 &fileId, const qint64 &userId, const QByteArray &data, const QString &mimeType);
    void remove(const qint64 &fileId, const qint64 userId);
    void clear();
    void setMemoryCacheSizeMb(int Kb);
    void setDiskCacheSizeMb(int Mb);
    QString getCachePath() { return m_cachePath; }

private:
    QString cacheKey(const qint64 &fileId, const qint64 &userId) const;
    QUrl cacheUrl(const QString &key) const;

private:
    mutable QCache<QString, QByteArray> m_memCache;
    QNetworkDiskCache *m_diskCache;
    QString m_cachePath = "";

};

#endif // FILECACHEMANAGER_H
