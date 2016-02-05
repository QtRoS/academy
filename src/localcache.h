#ifndef LOCALCACHE_H
#define LOCALCACHE_H

#include <QObject>
#include <QString>
#include <QByteArray>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QCryptographicHash>

class LocalCache
{
public:
    LocalCache(const QString& cacheDir);

    bool containsData(const QString& key) const;
    QByteArray data(const QString& key) const;
    bool setData(const QString& key, QByteArray data);

private:
    QString m_cacheDirectory;

private:
    QString fileNameByKey(const QString& key) const;
};

#endif // LOCALCACHE_H
