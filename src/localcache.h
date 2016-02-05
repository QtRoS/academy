#ifndef LOCALCACHE_H
#define LOCALCACHE_H

#include <QObject>
#include <QString>
#include <QByteArray>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <QCryptographicHash>
#include <QDateTime>
#include <QLoggingCategory>

Q_DECLARE_LOGGING_CATEGORY(LoCache)

class LocalCache
{
public:
    LocalCache(const QString& cacheDir);

    bool containsData(const QString& key) const;
    QByteArray data(const QString& key) const;
    bool setData(const QString& key, QByteArray data);

private:
    QString m_cacheDirectory;
    qint64 m_expireTime;

private:
    QString fileNameByKey(const QString& key) const;
};

#endif // LOCALCACHE_H
