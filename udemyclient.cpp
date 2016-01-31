#include <udemyclient.h>

#include <core/net/error.h>
#include <core/net/http/client.h>
#include <core/net/http/content_type.h>
#include <core/net/http/response.h>
#include <QVariantMap>

Q_LOGGING_CATEGORY(Udemy, "Udemy")

namespace http = core::net::http;
namespace net = core::net;

using namespace std;

UdemyClient::UdemyClient(Config::Ptr config) :
    BaseClient(config)
{ }

QList<Course> UdemyClient::courses(const QString &query)
{
    QList<Course> list;

    static QByteArray data;
    net::Uri::Path path;
    net::Uri::QueryParameters params;

    params.push_back({"fields[course]", "@default,description,headline,slug"});
    params.push_back({"page_size", "100"});

    qCDebug(Udemy) << "Download started...";
    if (data.isNull())
        get( path, params, data);
    qCDebug(Udemy) << "Data received:" << data.length() << "bytes";
    QJsonDocument root = QJsonDocument::fromJson(data);

    QVariantMap variant = root.toVariant().toMap();
    QList<QVariant> courses = variant["results"].toList();
    qCDebug(Udemy) << "Element count:" << courses.length();

    SearchEngine se(query);

    for (const QVariant &i : courses)
    {
        QVariantMap map = i.toMap();

        Course course;
        course.id = map["id"].toString();
        course.slug = map["url"].toString();
        course.title = map["title"].toString();
        course.subTitle = map["headline"].toString(); // .remove(QRegExp("<[^>]*>"));
        course.description = map["description"].toString();
        //course.shortDescription = map["short_summary"].toString();
        course.art = map["image_480x270"].toString();
        course.link = QStringLiteral("https://www.udemy.com") + map["url"].toString();
        //course.video = map["teaser_video"].toMap()["youtube_url"].toString();

        if (query.isEmpty() || se.isMatch(course))
            list.append(course);
    }

    return list;
}

const QString UdemyClient::baseApiUrl() const
{
    return QStringLiteral("https://www.udemy.com/api-2.0/courses");
}

const QString UdemyClient::name() const
{
    return QStringLiteral("Udemy");
}

QMap<QByteArray, QByteArray> UdemyClient::customHeaders() const
{
    QMap<QByteArray, QByteArray> res;
    res.insert("Authorization", "Basic MlloUmZ1TXpUSjJLMjJmZWZoSldTeVoyanVtOWx0dkdoWFhFUWZQaTpiNGRIUXhmUDdsODVWa3RHQlM4dUFpdU5ZclpyOEZWY3E3cFpTaWRXbVNMSTBuNm5mWGFyRUxSQ2xqdEtDbjZPcTR3ZkZwWjlqM0RsdU13aUhDN0UxVW1zS1YyQzRtSUlvR2ZEYXpNYVhtbDZjRGtHcjJmOHVqVzVkQ2J5VThaaw==");
    return res;
}
