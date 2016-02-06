#ifndef CACHEDCLIENT_H
#define CACHEDCLIENT_H

#include "baseclient.h"
#include "localcache.h"

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
