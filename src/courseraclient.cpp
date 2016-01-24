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
    BaseClient(config)
{

}

QList<CourseraClient::Course> CourseraClient::courses(const QString &query)
{
    QList<CourseraClient::Course> list;

    QByteArray data;
    net::Uri::Path path;

    net::Uri::QueryParameters params;
    params.push_back({"fields", "language,description,photoUrl,slug"});
    if (!query.isEmpty())
    {
        params.push_back({"q", "search"});
        params.push_back({"query", query.toStdString()});
    }
    get( path, params, data);
    QJsonDocument root = QJsonDocument::fromJson(data);

    QVariantMap variant = root.toVariant().toMap();
    QList<QVariant> elems = variant["elements"].toList();

    for (const QVariant &i : elems)
    {
        QVariantMap map = i.toMap();

        Course course;
        course.id = map["id"].toString();
        course.title = map["name"].toString();
        course.description = map["description"].toString();
        course.shortDescription = map["short_summary"].toString().left(100) + QStringLiteral("...");
        course.art = map["photoUrl"].toString();

        list.append(course);
    }

    // qDebug() << root.toJson();

    return list;
}

const QString CourseraClient::baseApiUrl() const
{
    return QStringLiteral("https://api.coursera.org/api/courses.v1");
}

const QString CourseraClient::name() const
{
    return QStringLiteral("Coursera");
}

