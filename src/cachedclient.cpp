#include "cachedclient.h"

CachedClient::CachedClient(Config::Ptr config):
    BaseClient(config),
    m_cache(QString::fromStdString(config->cache_dir))
{

}

void CachedClient::get(const core::net::Uri::Path &path, const core::net::Uri::QueryParameters &parameters, QByteArray &result)
{
    if (!m_cache.containsData(name()))
    {
        BaseClient::get(path, parameters, result);
        m_cache.setData(name(), result);
    }
    else result = m_cache.data(name());
}

