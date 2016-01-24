#ifndef QUERY_H_
#define QUERY_H_

#include <client.h>

#include <unity/scopes/SearchQueryBase.h>
#include <unity/scopes/ReplyProxyFwd.h>

#include "config.h"
#include "courseraclient.h"
#include "udacityclient.h"
#include "edxclient.h"

/**
 * Represents an individual query.
 *
 * A new Query object will be constructed for each query. It is
 * given query information, metadata about the search, and
 * some scope-specific configuration.
 */
class Query: public unity::scopes::SearchQueryBase
{
public:
    Query(const unity::scopes::CannedQuery &query, const unity::scopes::SearchMetadata &metadata, Config::Ptr config);

    ~Query() = default;

    void cancelled() override;

    void run(const unity::scopes::SearchReplyProxy &reply) override;

private:
    Client client_;
    Config::Ptr m_config;
};

#endif // QUERY_H_


