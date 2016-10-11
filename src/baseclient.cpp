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

#include <baseclient.h>

#include <core/net/error.h>
#include <core/net/http/client.h>
#include <core/net/http/content_type.h>
#include <core/net/http/response.h>
#include <QVariantMap>

#include <json/json.h>

Q_LOGGING_CATEGORY(BaseCli, "BaseClient")

namespace http = core::net::http;
namespace net = core::net;
namespace json = Json;

using namespace std;

BaseClient::BaseClient(Config::Ptr config) :
    m_config(config), m_cancelled(false)
{

}

const BaseClient::header_list BaseClient::customHeaders() const
{
    return header_list();
}

void BaseClient::get(const net::Uri::Path &path, const net::Uri::QueryParameters &parameters, QByteArray &result)
{
    // Create a new HTTP client
    auto client = http::make_client();

    // Start building the request configuration
    http::Request::Configuration configuration;

    // Build the URI from its components
    net::Uri uri = net::make_uri(baseApiUrl(), path, parameters);
    configuration.uri = client->uri_to_string(uri);

    // Give out a user agent string
    configuration.header.add("User-Agent", m_config->user_agent);

    BaseClient::header_list headers = customHeaders();
    for(auto header : headers)
        configuration.header.add(header.first, header.second);

    // Build a HTTP request object from our configuration
    auto request = client->head(configuration);

    try {
        // Synchronously make the HTTP request
        // We bind the cancellable callback to #progress_report
        auto response = request->execute(bind(&BaseClient::progress_report, this, placeholders::_1));

        // Check that we got a sensible HTTP status code
        if (response.status != http::Status::ok) {
            throw domain_error(response.body);
        }

        result = response.body.c_str();
        // qDebug() << response.body.c_str();

    } catch (net::Error &e) {
        qCWarning(BaseCli) << "NetworkError:" << e.what();
    }
}

http::Request::Progress::Next BaseClient::progress_report(const http::Request::Progress&)
{
    return m_cancelled ? http::Request::Progress::Next::abort_operation : http::Request::Progress::Next::continue_operation;
}

void BaseClient::cancel()
{
    m_cancelled = true;
    qCDebug(BaseCli) << QString::fromStdString(name()) << "cancelled";
}

Config::Ptr BaseClient::config()
{
    return m_config;
}

