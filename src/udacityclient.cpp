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
    BaseClient(config)
{

}

QList<UdacityClient::Course> UdacityClient::courses(const QString &query)
{
    QList<UdacityClient::Course> list;

    QJsonDocument root;
    net::Uri::Path path;
    net::Uri::QueryParameters params;

    get( path, params, root);

    QVariantMap variant = root.toVariant().toMap();
    QList<QVariant> courses = variant["courses"].toList();

    for (const QVariant &i : courses)
    {
        QVariantMap map = i.toMap();

        Course course;
        course.id = map["key"].toString();
        course.title = map["title"].toString();
        course.subTitle = map["subtitle"].toString();
        course.description = map["summary"].toString();
        course.shortDescription = map["short_summary"].toString();
        course.art = map["image"].toString();

        list.append(course);
    }

    // qDebug() << root.toJson();

    return list;
}

const QString UdacityClient::baseApiUrl() const
{
    return QStringLiteral("https://www.udacity.com/public-api/v0/courses");
}

const QString UdacityClient::name() const
{
    return QStringLiteral("Udacity");
}
