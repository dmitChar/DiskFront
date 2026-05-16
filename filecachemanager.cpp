#include "filecachemanager.h"
#include <QBuffer>
#include <QIODevice>
#include <QDebug>
#include <QCryptographicHash>


FileCacheManager::FileCacheManager(const QString &cacheDir, QObject *parent):
    QObject (parent),
    m_memCache(32 * 1024 * 1024), // 32 Mb
    m_diskCache(new QNetworkDiskCache(this))
{
    m_diskCache->setCacheDirectory(cacheDir);
    m_diskCache->setMaximumCacheSize(512 * 1024 * 1024);
}

optional<QByteArray> FileCacheManager::get(const qint64 &fileId, const qint64 &userId) const
{
    QString key = cacheKey(fileId, userId);

    // ----- L1 память -----
    if (QByteArray *cached = m_memCache.object(key))
    {
        qDebug() << "[CacheManager] L1 HIT" << fileId;
        return *cached;
    }

    // ----- L2 диск -----
    QIODevice *dev = m_diskCache->data(cacheUrl(key));
    if (dev)
    {
        QByteArray data = dev->readAll();
        delete dev;

        if (!data.isEmpty())
        {
            qDebug() << "[CacheManager] L2 HIT" << fileId << data.size() << "bytes";
            // Продвигаем в L1
            m_memCache.insert(key, new QByteArray(data), data.size());
            return data;
        }
    }
    qDebug() << "[CacheManager] MISS" << fileId;
    return nullopt;
}

void FileCacheManager::put(const qint64 &fileId, const qint64 &userId, const QByteArray &data, const QString &mimeType)
{
    if (data.isEmpty())
        return;

    QString key = cacheKey(fileId, userId);

    // ----- L1 память -----
    m_memCache.insert(key, new QByteArray(data), data.size());

    // ----- L2 диск -----
    QNetworkCacheMetaData meta;
    meta.setUrl(cacheUrl(key));
    meta.setSaveToDisk(true);

    // Установка Content-Type
    if (!mimeType.isEmpty())
    {
        QNetworkCacheMetaData::RawHeaderList headers;
        headers.append(qMakePair(QByteArray("Content-Type"), mimeType.toUtf8()));
        meta.setRawHeaders(headers);
    }

    QIODevice *dev = m_diskCache->prepare(meta);
    if (dev)
    {
        dev->write(data);
        m_diskCache->insert(dev);
        qDebug() << "[CacheManager] PUT" << fileId << data.size() << "bytes";
    }
}

void FileCacheManager::remove(const qint64 &fileId, const qint64 userId)
{
    QString key = cacheKey(fileId, userId);
    m_memCache.remove(key);
    m_diskCache->remove(cacheUrl(key));
}

void FileCacheManager::clear()
{
    m_memCache.clear();
    m_diskCache->clear();
}

void FileCacheManager::setMemoryCacheSizeMb(int Kb)
{
    m_memCache.setMaxCost(Kb * 1024);
}

void FileCacheManager::setDiskCacheSizeMb(int Mb)
{
    m_diskCache->setMaximumCacheSize(Mb * 1024 * 1024);
}

qint64 FileCacheManager::memoryCacheUsedBytes() const
{
    return m_memCache.totalCost();
}

qint64 FileCacheManager::diskCacheUsedBytes() const
{
    return m_diskCache->cacheSize();
}

QString FileCacheManager::cacheKey(const qint64 &fileId, const qint64 &userId) const
{
    QString raw = QString("%1:%2").arg(userId).arg(fileId);
    return QCryptographicHash::hash(raw.toUtf8(), QCryptographicHash::Sha256).toHex();
}

QUrl FileCacheManager::cacheUrl(const QString &key) const
{
    return QUrl("cache://" + key);
}


