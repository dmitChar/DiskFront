#ifndef USERMODEL_H
#define USERMODEL_H

#include <QObject>
#include <QString>

class UserModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(qint64  userId      READ userId      NOTIFY userChanged)
    Q_PROPERTY(QString username    READ username    NOTIFY userChanged)
    Q_PROPERTY(QString email       READ email       NOTIFY userChanged)
    Q_PROPERTY(qint64  quotaBytes  READ quotaBytes  NOTIFY quotaChanged)
    Q_PROPERTY(qint64  usedBytes   READ usedBytes   NOTIFY quotaChanged)
    Q_PROPERTY(qint64  freeBytes   READ freeBytes   NOTIFY quotaChanged)
    Q_PROPERTY(int     usedPercent READ usedPercent NOTIFY quotaChanged)
    Q_PROPERTY(QString humanQuota  READ humanQuota  NOTIFY quotaChanged)
    Q_PROPERTY(QString humanUsed   READ humanUsed   NOTIFY quotaChanged)


public:
    explicit UserModel(QObject *parent = nullptr);

    qint64   userId()     const  {return m_userId;}
    QString  username()  const   {return m_username;}
    QString  email()     const   {return m_email;}
    qint64   quotaBytes() const  {return m_quotaBytes;}
    qint64   usedBytes() const   {return m_usedBytes;}
    qint64   freeBytes()  const  {return m_quotaBytes - m_usedBytes;}
    int      usedPercent() const {return m_quotaBytes > 0 ? int(m_usedBytes * 100 /m_quotaBytes) : 0;}
    QString  humanQuota()  const  {return humanize(m_quotaBytes);}
    QString  humanUsed()   const {return humanize(m_usedBytes);}

public:
    void setUserFromJson(const QJsonObject &obj);
    void setQuotaFromJson(const QJsonObject &obj);
    void clear();

private:
    qint64  m_userId = 0;
    QString m_username;
    QString m_email;
    qint64  m_quotaBytes = 0;
    qint64  m_usedBytes = 0;

    static QString humanize(qint64 bytes);

signals:
    void userChanged();
    void quotaChanged();

public slots:
};

#endif // USERMODEL_H
