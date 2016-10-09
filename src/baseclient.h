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

#ifndef BASECLIENT_H_
#define BASECLIENT_H_

#include <atomic>
#include <deque>
#include <map>
#include <memory>
#include <string>
#include <core/net/http/request.h>
#include <core/net/uri.h>

#include <QJsonDocument>
#include <QString>
#include <QList>
#include <QDebug>
#include <QMap>
#include <QByteArray>

#include <QLoggingCategory>

#include "config.h"
#include "course.h"

Q_DECLARE_LOGGING_CATEGORY(BaseCli)

/**
 * Provide a nice way to access the HTTP API.
 */
class BaseClient
{
public:
    BaseClient(Config::Ptr config);

    virtual ~BaseClient() = default;

    /**
     * @brief main method of a client class, it is used when Query is trying to retrieve results
     * @param query - search query, keyword or course name (from user input)
     * @return list of available courses
     */
    virtual vector<Course> courses(const string& query) = 0;

    /**
     * @brief this method is used by a client when it is contructing full URL (with parameters)
     * @return base URL of your service
     */
    virtual const string baseApiUrl() const = 0;

    /**
     * @brief used by Query when it is organazing results in groups
     * @return human-visible name of a client
     */
    virtual const string name() const = 0;

    /**
     * @brief Some APIs require custom headers in HTTP request (for example for authorization)
     * @return map of headers
     */
    virtual const QMap<QByteArray, QByteArray> customHeaders() const;

    /**
     * Cancel any pending queries (this method can be called from a different thread)
     */
    virtual void cancel();

    virtual Config::Ptr config();

protected:

    /**
     * @brief This method is used to get raw data (QByteArray) from API. Basically you DO NOT need to override it
     * @param path - additional path in API URL, returned by baseApiUrl()
     * @param parameters - parameter pairs (the will be automatically transformed to HTTP parameters)
     * @param result - raw bytes of result
     */
    virtual void get(const core::net::Uri::Path &path, const core::net::Uri::QueryParameters &parameters, QByteArray &result);

    /**
     * Progress callback that allows the query to cancel pending HTTP requests.
     */
    core::net::http::Request::Progress::Next progress_report(const core::net::http::Request::Progress& progress);

    /**
     * Hang onto the configuration information
     */
    Config::Ptr m_config;

    /**
     * Thread-safe cancelled flag
     */
    std::atomic<bool> m_cancelled;
};

#endif // BASECLIENT_H_

