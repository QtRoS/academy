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

#ifndef UDACITYCLIENT_H_
#define UDACITYCLIENT_H_

#include <core/net/http/request.h>
#include <core/net/uri.h>

#include <QString>

#include "config.h"
#include "baseclient.h"
#include "cachedclient.h"
#include "searchengine.h"

Q_DECLARE_LOGGING_CATEGORY(Udacity)

class UdacityClient : public CachedClient
{
public:

    UdacityClient(Config::Ptr config);
    virtual ~UdacityClient() = default;

    virtual vector<Course> courses(const string& query) override;
    virtual const string baseApiUrl() const override;
    virtual const string name() const override;

private:
    QString grabExtra(const QVariantMap& map);
};

#endif // UDACITYCLIENT_H_

