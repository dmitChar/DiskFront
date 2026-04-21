#ifndef APIRESPONSE_H
#define APIRESPONSE_H

#include <QString>
#include <QJsonObject>
#include <QJsonArray>
#include <optional>
#include <QMap>

using std::optional;
using std::nullopt;

struct ApiResponse
{
    bool succes = false;
    int httpCode = 0;
    QString message;
    QJsonObject data;
    QJsonArray dataArray;
    bool isArray = false;
    QString errorMsg;

    static ApiResponse ok(const QJsonObject &d, const QString &msg = {})
    {
        ApiResponse r;
        r.succes = true;
        r.data = d;
        r.message = msg;
        return r;
    }

    static ApiResponse okArray(const QJsonArray &arr, const QString &msg = {})
    {
        ApiResponse r;
        r.succes = true;
        r.dataArray = arr;
        r.isArray = true;
        r.message = msg;
        return r;
    }

    static ApiResponse fail(const QString &err, int code = 0)
    {
        ApiResponse r;
        r.succes = false;
        r.errorMsg = err;
        r.httpCode = code;
        return r;
    }

    static QString extensionFromMimeType(const QString &mimeType)
    {
        static const QMap<QString, QString> mimeMap =
        {
            {"image/jpeg", "jpg"}, {"image/jpeg", "jpeg"}, {"image/png", "png"},
            {"image/gif", "gif"},  {"image/bmp", "bmp"},   {"image/webp", "webp"},
            {"image/svg+xml", "svg"}, {"application/pdf", "pdf"},
            {"text/x-c", "cpp"}, {"text/plain", ".h"},
            {"text/plain", "txt"}, {"text/markdown", "md"}, {"text/csv", "csv"},
            {"text/html", "html"}, {"text/css", "css"}, {"application/javascript", "js"},
            {"application/json", "json"}, {"application/xml", "xml"},
            {"application/zip", "zip"}, {"application/x-tar", "tar"},
            {"application/gzip", "gz"}, {"application/x-7z-compressed", "7z"},
            {"application/vnd.rar", "rar"}, {"audio/mpeg", "mp3"},
            {"video/mp4", "mp4"}, {"video/x-msvideo", "avi"},
            {"video/quicktime", "mov"}, {"video/x-matroska", "mkv"},
            {"application/msword", "doc"}, {"application/vnd.ms-excel", "xls"},
            {"application/vnd.openxmlformats-officedocument.wordprocessingml.document", "docx"},
            {"application/vnd.openxmlformats-officedocument.spreadsheetml.sheet", "xlsx"},
            {"application/vnd.ms-powerpoint", "ppt"},
            {"application/vnd.openxmlformats-officedocument.presentationml.presentation", "pptx"},
        };

        QString type = mimeMap.value(mimeType);
        return mimeMap.value(mimeType);
    }
};



#endif // APIRESPONSE_H
