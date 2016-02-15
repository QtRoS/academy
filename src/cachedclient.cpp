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

