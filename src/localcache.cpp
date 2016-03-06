/*
 * Copyright (C) 2016 Roman Shchekin aka mrqtros.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "localcache.h"

Q_LOGGING_CATEGORY(LoCache, "LocalCache")

LocalCache::LocalCache(const QString &cacheDir):
    m_cacheDirectory(cacheDir),
    m_expireTime(60 * 60 *24)
{

}

bool LocalCache::containsData(const QString &key) const
{
    QString filePath = fileNameByKey(key);
    QFileInfo fi(filePath);

    if (!fi.exists())
        return false;

    QDateTime now = QDateTime::currentDateTime();
    // qCDebug(LoCache) << now << fi.lastModified() << fi.lastModified().secsTo(now);
    return fi.lastModified().secsTo(now) < m_expireTime;
}

QByteArray LocalCache::data(const QString &key) const
{
    QString filePath = fileNameByKey(key);

    QFile file(filePath);
    if (!file.open(QFile::ReadOnly))
    {
        qCDebug(LoCache) << "Can't read data file: " << filePath;
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
        qCDebug(LoCache) << "Can't write data file: " << filePath;
        return false;
    }

    QTextStream stream(&file);
    stream << data;
    return true;
}

QString LocalCache::fileNameByKey(const QString &key) const
{
    QString md5 = QCryptographicHash::hash(key.toLatin1(), QCryptographicHash::Md5).toHex();
    return QDir::cleanPath(m_cacheDirectory + QDir::separator() + md5 + FileNameSuffix);
}
