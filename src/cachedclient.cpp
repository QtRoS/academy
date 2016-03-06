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

#include "cachedclient.h"

CachedClient::CachedClient(Config::Ptr config):
    BaseClient(config),
    m_cache(QString::fromStdString(config->cache_dir)),
    m_cacheEnabled(true)
{

}

void CachedClient::get(const core::net::Uri::Path &path, const core::net::Uri::QueryParameters &parameters, QByteArray &result)
{
    if (m_cacheEnabled)
    {
        if (!m_cache.containsData(name()))
        {
            BaseClient::get(path, parameters, result);
            if (!m_cancelled)
                m_cache.setData(name(), result);
        }
        else result = m_cache.data(name());
    }
    else BaseClient::get(path, parameters, result);

    // Less clear version.
//    if (!m_cacheEnabled || !m_cache.containsData(name()))
//        BaseClient::get(path, parameters, result);

//    if (m_cacheEnabled)
//    {
//        if (!m_cache.containsData(name()))
//            m_cache.setData(name(), result);
//        else result = m_cache.data(name());
//    }
}

