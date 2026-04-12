#ifndef FILEINFO_H
#define FILEINFO_H
#include <QString>

struct FileInfo
{
    QString id;
    QString name;
    qint64 size;
    QString createdAt;
    QString iconSrc;
};
#endif // FILEINFO_H

//увеличить пул кодов команд
