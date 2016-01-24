#include <udacityclient.h>

#include <core/net/error.h>
#include <core/net/http/client.h>
#include <core/net/http/content_type.h>
#include <core/net/http/response.h>
#include <QVariantMap>

Q_LOGGING_CATEGORY(Udacity, "Udacity")

namespace http = core::net::http;
namespace net = core::net;

using namespace std;

UdacityClient::UdacityClient(Config::Ptr config) :
    BaseClient(config)
{ }

QList<Course> UdacityClient::courses(const QString &query)
{
    QList<Course> list;

    QByteArray data;
    net::Uri::Path path;
    net::Uri::QueryParameters params;

    get( path, params, data);
    qCDebug(Udacity) << "Data received:" << data.length() << "bytes";
    QJsonDocument root = QJsonDocument::fromJson(data);

    QVariantMap variant = root.toVariant().toMap();
    QList<QVariant> courses = variant["courses"].toList();
    qCDebug(Udacity) << "Element count:" << courses.length();

    SearchEngine se(query);

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

        if (query.isEmpty() || se.isMatch(course))
            list.append(course);
    }

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
