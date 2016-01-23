#include <courseraclient.h>

#include <core/net/error.h>
#include <core/net/http/client.h>
#include <core/net/http/content_type.h>
#include <core/net/http/response.h>
#include <QVariantMap>

namespace http = core::net::http;
namespace net = core::net;

using namespace std;

CourseraClient::CourseraClient(Config::Ptr config) :
    m_config(config), m_cancelled(false)
{

}

QList<CourseraClient::Course> CourseraClient::courses(const QString &query)
{
    QList<CourseraClient::Course> list;

    Course course;
    course.id = "1";
    course.name = "MisZiKi";
    course.description = "Badly designed course (like all others)";
    course.art = "https://pp.vk.me/c425218/v425218832/d93/zOGI9yhcGwk.jpg";
    course.university = "PGU";
    course.lector = "Zinkin";

    list.append(course);
    list.append(course);

    return list;
}


void CourseraClient::get(const net::Uri::Path &path, const net::Uri::QueryParameters &parameters, QJsonDocument &root)
{
    // Create a new HTTP client
    auto client = http::make_client();

    // Start building the request configuration
    http::Request::Configuration configuration;

    // Build the URI from its components
    net::Uri uri = net::make_uri(m_config->apiroot, path, parameters);
    configuration.uri = client->uri_to_string(uri);

    // Give out a user agent string
    configuration.header.add("User-Agent", m_config->user_agent);

    // Build a HTTP request object from our configuration
    auto request = client->head(configuration);

    try {
        // Synchronously make the HTTP request
        // We bind the cancellable callback to #progress_report
        auto response = request->execute(bind(&CourseraClient::progress_report, this, placeholders::_1));

        // Check that we got a sensible HTTP status code
        if (response.status != http::Status::ok) {
            throw domain_error(response.body);
        }
        // Parse the JSON from the response
        root = QJsonDocument::fromJson(response.body.c_str());

        // Open weather map API error code can either be a string or int
        QVariant cod = root.toVariant().toMap()["cod"];
        if ((cod.canConvert<QString>() && cod.toString() != "200")
                || (cod.canConvert<unsigned int>() && cod.toUInt() != 200)) {
            throw domain_error(root.toVariant().toMap()["message"].toString().toStdString());
        }
    } catch (net::Error &) {
    }
}

http::Request::Progress::Next CourseraClient::progress_report(const http::Request::Progress&)
{
    return m_cancelled ? http::Request::Progress::Next::abort_operation : http::Request::Progress::Next::continue_operation;
}

void CourseraClient::cancel()
{
    m_cancelled = true;
}

Config::Ptr CourseraClient::config()
{
    return m_config;
}

