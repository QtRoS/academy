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

#ifndef CACHEDCLIENT_H
#define CACHEDCLIENT_H

#include "baseclient.h"
#include "localcache.h"

/**
 * @brief The CachedClient class introduces caching capabilities.
 */
class CachedClient : public BaseClient
{
public:
    CachedClient(Config::Ptr config);

protected:
    virtual void get(const core::net::Uri::Path &path, const core::net::Uri::QueryParameters &parameters, QByteArray &result) override;

    bool cacheEnabled() const { return m_cacheEnabled; }
    void setCacheEnabled(bool v) { m_cacheEnabled = v; }

    LocalCache m_cache;
    bool m_cacheEnabled;
};

#endif // CACHEDCLIENT_H
