#include <baseclient.h>

#include <core/net/error.h>
#include <core/net/http/client.h>
#include <core/net/http/content_type.h>
#include <core/net/http/response.h>
#include <QVariantMap>

Q_LOGGING_CATEGORY(BaseCli, "BaseClient")

namespace http = core::net::http;
namespace net = core::net;

using namespace std;

BaseClient::BaseClient(Config::Ptr config) :
    m_config(config), m_cancelled(false)
{

}

const QMap<QByteArray, QByteArray> BaseClient::customHeaders() const
{
    return QMap<QByteArray, QByteArray>();
}

void BaseClient::get(const net::Uri::Path &path, const net::Uri::QueryParameters &parameters, QByteArray &result)
{
    // Create a new HTTP client
    auto client = http::make_client();

    // Start building the request configuration
    http::Request::Configuration configuration;

    // Build the URI from its components
    net::Uri uri = net::make_uri(/*m_config->apiroot*/ baseApiUrl().toStdString(), path, parameters); // TODO
    configuration.uri = client->uri_to_string(uri);

    // Give out a user agent string
    configuration.header.add("User-Agent", m_config->user_agent);

    QMap<QByteArray, QByteArray> headers = customHeaders();
    for(auto key : headers.keys())
        configuration.header.add(key.toStdString(), headers.value(key).toStdString());

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
        //qDebug() << response.body.c_str();

//        QVariant cod = root.toVariant().toMap()["cod"];
//        if ((cod.canConvert<QString>() && cod.toString() != "200")
//                || (cod.canConvert<unsigned int>() && cod.toUInt() != 200)) {
//            throw domain_error(root.toVariant().toMap()["message"].toString().toStdString());
//        }

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
    qCDebug(BaseCli) << "Cancelled";
}

Config::Ptr BaseClient::config()
{
    return m_config;
}

