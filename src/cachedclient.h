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

private:
    LocalCache m_cache;
};

#endif // CACHEDCLIENT_H
