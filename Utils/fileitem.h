#ifndef FILEITEM_H
#define FILEITEM_H

#include <QJsonArray>
#include <QJsonObject>
#include <QDateTime>
#include <QVariant>

struct FileItem
{
    qint64 id;
    QString name;
    QString path;
    QString type;    // "file" | "directory"
    qint64 sizeBytes;
    QString mimeType;
    QString checkSum;
    bool isShared = false;
    QString shareToken;
    QDateTime createdAt;
    QDateTime updatedAt;
    QDateTime lastAccessed;

    bool isDir() const
    {
        return type == "directory";
    }

    static FileItem fromJson(const QJsonObject &obj)
    {
        FileItem f;
        f.id        = obj["id"].toVariant().toLongLong();
        f.name      = obj["name"].toString();
        f.path      = obj["path"].toString();
        f.type      = obj["type"].toString("file");
        f.sizeBytes = obj["sizeBytes"].toVariant().toLongLong();
        f.mimeType  = obj["mimeType"].toString();
        f.checkSum  = obj["checkSum"].toString();
        f.isShared  = obj["isShared"].toBool();
        f.shareToken= obj["shareToken"].toString();
        f.createdAt = QDateTime::fromString(obj["createdAt"].toString(), Qt::ISODate);
        f.updatedAt = QDateTime::fromString(obj["updatedAt"].toString(), Qt::ISODate);
        f.lastAccessed = QDateTime::fromString(obj["lastAccessed"].toString(), Qt::ISODate);
        return f;
    }

    QString humanSize() const
    {
        if (isDir()) return "";
        if (sizeBytes < 1024) return QString("%1 B").arg(sizeBytes);
        if (sizeBytes < 1024 * 1024) return QString ("%1 KB").arg(sizeBytes / 1024.0, 0, 'f', 1);
        if (sizeBytes < 1024 * 1024  * 1024) return QString("%1 MB").arg(sizeBytes / 1024.0 / 1024.0, 0, 'f', 1);
        return QString("%1 GB").arg(sizeBytes / 1024.0 / 1024.0 / 1024.0, 0, 'f', 2);
    }
    QString iconName() const
    {
        if (isDir()) return "folder";
        if (mimeType.startsWith("image/")) return "image";
        if (mimeType.startsWith("video/")) return "video";
        if (mimeType.startsWith("audio/")) return "audio";
        if (mimeType == "application/pdf") return "pdf";
        if (mimeType.contains("zip") || mimeType.contains("tar") || mimeType.contains("gzip") || mimeType.contains("7z")) return "archive";
        if (mimeType.contains("word") || name.endsWith(".doc") || name.endsWith(".docx")) return "document";
        if (mimeType.contains("spreadsheet") || name.endsWith(".xls") || name.endsWith("xlsx")) return "spreadsheet";
        if (mimeType.contains("presentation") || name.endsWith(".ppt") || name.endsWith("pptx")) return "presentation";
        if (mimeType.startsWith("text/")) return "text";
        return "file";
    }
};

#endif // FILEITEM_H
