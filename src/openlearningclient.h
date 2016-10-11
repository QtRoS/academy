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

#ifndef OPENLEARNINGCLIENT_H_
#define OPENLEARNINGCLIENT_H_

#include <core/net/http/request.h>
#include <core/net/uri.h>

#include "config.h"
#include "cachedclient.h"
#include "searchengine.h"

Q_DECLARE_LOGGING_CATEGORY(OpenLearning)

class OpenLearningClient : public CachedClient
{
public:

    OpenLearningClient(Config::Ptr config);
    virtual ~OpenLearningClient() = default;

    virtual vector<Course> courses(const string& query) override;
    virtual const string baseApiUrl() const override;
    virtual const string name() const override;

private:
    string grabVideo(const string& promo);
};

#endif // OPENLEARNINGCLIENT_H_

