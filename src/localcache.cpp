#include "localcache.h"


LocalCache::LocalCache(const QString &cacheDir):
    m_cacheDirectory(cacheDir)
{

}

bool LocalCache::containsData(const QString &key) const
{
    QString filePath = fileNameByKey(key);
    return QFile::exists(filePath);
}

QByteArray LocalCache::data(const QString &key) const
{
    QString filePath = fileNameByKey(key);

    QFile file(filePath);
    if (!file.open(QFile::ReadOnly))
    {
        // TODO Log
        return QByteArray();
    }

    return file.readAll();
}

bool LocalCache::setData(const QString &key, QByteArray data)
{
    QString filePath = fileNameByKey(key);

    QFile file(filePath);
    if (!file.open(QFile::WriteOnly))
    {
        // TODO Log
        return false;
    }

    QTextStream stream(&file);
    stream << data;
    return true;
}

QString LocalCache::fileNameByKey(const QString &key) const
{
    QString md5 = QCryptographicHash::hash(key.toLatin1(), QCryptographicHash::Md5).toHex();
    return QDir::cleanPath(m_cacheDirectory + QDir::separator() + md5);
}
