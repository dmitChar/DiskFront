#ifndef FILECACHEMANAGER_H
#define FILECACHEMANAGER_H

#include <QObject>
#include <QCache>
#include <QByteArray>
#include <QNetworkCacheMetaData>
#include <QNetworkDiskCache>
#include <optional>
#include <QUrl>

using std::optional;
using std::nullopt;

// Двухуровневый кеш медиа-данных:
//   L1 — память:  QCache<QString, QByteArray>   (быстро, ограничен по размеру)
//   L2 — диск:    QNetworkDiskCache             (медленнее, переживает перезапуск)
//
// Ключ кеша — SHA-256(fileId + userId)
class FileCacheManager : public QObject
{
    Q_OBJECT
public:
    explicit FileCacheManager(const QString &cacheDir, QObject *parent = nullptr);

    // ----- Поиск -----
    // Возвращает данные если есть хотя бы в одном уровне
    optional<QByteArray> get(const qint64 &fileId, const qint64 &userId) const;

    // ----- Запись -----
    // Пишет сразу в оба уровня
    void put(const qint64 &fileId, const qint64 &userId, const QByteArray &data, const QString &mimeType = {});

    // ----- Инвалидация -----
    void remove(const qint64 &fileId, const qint64 userId);
    void clear();

    // ----- Настройки -----
    void setMemoryCacheSizeMb(int Kb); // По умолчанию 32 Мб
    void setDiskCacheSizeMb(int Mb);   // По умолчанию 512 Мб

    // ----- Статистика -----
    qint64 memoryCacheUsedBytes() const;
    qint64 diskCacheUsedBytes() const;

private:
    QString cacheKey(const qint64 &fileId, const qint64 &userId) const;
    QUrl cacheUrl(const QString &key) const;

    // L1: память — авто-удаляет старые при переполнении
    mutable QCache<QString, QByteArray> m_memCache;

    // L2: диск — через стандартный Qt disk cache
    QNetworkDiskCache *m_diskCache;
};

#endif // FILECACHEMANAGER_H
