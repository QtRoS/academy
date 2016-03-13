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

#ifndef QUERY_H_
#define QUERY_H_

#include <unity/scopes/SearchQueryBase.h>
#include <unity/scopes/ReplyProxyFwd.h>
#include <unity/scopes/Department.h>

#include <QLoggingCategory>
#include <QPointer>
#include <QSharedPointer>
#include <QSet>

#include "config.h"
#include "courseraclient.h"
#include "udacityclient.h"
#include "edxclient.h"
#include "udemyclient.h"
#include "iversityclient.h"
#include "openlearningclient.h"

#include "departmentmanager.h"

Q_DECLARE_LOGGING_CATEGORY(Qry)

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
    //Client client_;
    Config::Ptr m_config;
    CourseraClient m_coursera;
    UdemyClient m_udemy;
    EdxClient m_edx;
    UdacityClient m_udacity;
    IversityClient m_iversity;
    OpenLearningClient m_openLearning;
};

#endif // QUERY_H_


