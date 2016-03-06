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

    qint64 expireTime() const { return m_expireTime; }
    void setExpireTime(qint64 t) { m_expireTime = t; }
    const QString FileNameSuffix = ".v1";

private:
    QString m_cacheDirectory;
    qint64 m_expireTime;

private:
    QString fileNameByKey(const QString& key) const;
};

#endif // LOCALCACHE_H
