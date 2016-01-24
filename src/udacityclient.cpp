#include <udacityclient.h>

#include <core/net/error.h>
#include <core/net/http/client.h>
#include <core/net/http/content_type.h>
#include <core/net/http/response.h>
#include <QVariantMap>

namespace http = core::net::http;
namespace net = core::net;

using namespace std;

UdacityClient::UdacityClient(Config::Ptr config) :
    m_config(config), m_cancelled(false)
{

}

QList<UdacityClient::Course> UdacityClient::courses(const QString &query)
{
    QList<UdacityClient::Course> list;

    QJsonDocument root;
    net::Uri::Path path;

    net::Uri::QueryParameters params;
    params.push_back({"fields", "language,description,photoUrl"});
    if (!query.isEmpty())
    {
        params.push_back({"q", "search"});
        params.push_back({"query", query.toStdString()});
    }
    get( path, params, root);
    QVariantMap variant = root.toVariant().toMap();
    QList<QVariant> elems = variant["elements"].toList();

    for (const QVariant &i : elems)
    {
        QVariantMap map = i.toMap();

        Course course;
        course.id = map["id"].toString();
        course.name = map["name"].toString();
        course.description = map["description"].toString();
        course.art = map["photoUrl"].toString();

        list.append(course);
    }

    // qDebug() << root.toJson();

    return list;
}


void UdacityClient::get(const net::Uri::Path &path, const net::Uri::QueryParameters &parameters, QJsonDocument &root)
{
    // Create a new HTTP client
    auto client = http::make_client();

    // Start building the request configuration
    http::Request::Configuration configuration;

    // Build the URI from its components
    net::Uri uri = net::make_uri(/*m_config->apiroot*/ "https://api.coursera.org/api/courses.v1", path, parameters); // TODO
    configuration.uri = client->uri_to_string(uri);

    // Give out a user agent string
    configuration.header.add("User-Agent", m_config->user_agent);

    // Build a HTTP request object from our configuration
    auto request = client->head(configuration);

    try {
        // Synchronously make the HTTP request
        // We bind the cancellable callback to #progress_report
        auto response = request->execute(bind(&UdacityClient::progress_report, this, placeholders::_1));

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

http::Request::Progress::Next UdacityClient::progress_report(const http::Request::Progress&)
{
    return m_cancelled ? http::Request::Progress::Next::abort_operation : http::Request::Progress::Next::continue_operation;
}

void UdacityClient::cancel()
{
    m_cancelled = true;
}

Config::Ptr UdacityClient::config()
{
    return m_config;
}

