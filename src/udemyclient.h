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

#ifndef UDEMYCLIENT_H_
#define UDEMYCLIENT_H_

#include <core/net/http/request.h>
#include <core/net/uri.h>

#include <QJsonDocument>
#include <QJsonObject>
#include <QString>
#include <QList>
#include <QMap>
#include <QByteArray>
#include <QDebug>

#include "config.h"
#include "baseclient.h"
#include "searchengine.h"
#include "cachedclient.h"

Q_DECLARE_LOGGING_CATEGORY(Udemy)

class UdemyClient : public CachedClient
{
public:

    UdemyClient(Config::Ptr config);
    virtual ~UdemyClient() = default;

    virtual QList<Course> courses(const QString& query) override;
    virtual const QString baseApiUrl() const override;
    virtual const QString name() const override;
    virtual const QMap<QByteArray, QByteArray> customHeaders() const override;

private:
    QString grabExtra(const QVariantMap& map);
};

#endif // UDEMYCLIENT_H_

