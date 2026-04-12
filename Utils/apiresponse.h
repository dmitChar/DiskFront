#ifndef APIRESPONSE_H
#define APIRESPONSE_H

#include <QString>
#include <QJsonObject>
#include <QJsonArray>
#include <experimental/optional>

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
};



#endif // APIRESPONSE_H
