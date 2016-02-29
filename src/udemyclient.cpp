#include <udemyclient.h>
#include <localization.h>

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
    CachedClient(config)
{ }

QList<Course> UdemyClient::courses(const QString &query)
{
    QList<Course> list;

    QByteArray data;
    net::Uri::Path path;
    net::Uri::QueryParameters params;

    params.push_back({"fields[course]", "@default,description,headline,slug,primary_category"});
    params.push_back({"page_size", "80"});
    params.push_back({"ordering", "trending"});

    if (!query.isEmpty())
    {
        params.push_back({"search", query.toStdString()});
        setCacheEnabled(false);
    }

    qCDebug(Udemy) << "Download started...";
    if (data.isNull())
        get( path, params, data);
    qCDebug(Udemy) << "Data received:" << data.length() << "bytes";
    QJsonDocument root = QJsonDocument::fromJson(data);

    QVariantMap variant = root.toVariant().toMap();
    QList<QVariant> courses = variant["results"].toList();
    qCDebug(Udemy) << "Element count:" << courses.length();

    for (const QVariant &i : courses)
    {
        QVariantMap map = i.toMap();

        Course course;
        course.id = map["id"].toString();
        course.slug = map["url"].toString();
        course.title = map["title"].toString();
        course.description = map["description"].toString();
        course.headline = map["headline"].toString();
        course.art = map["image_480x270"].toString();
        course.link = QStringLiteral("https://www.udemy.com") + map["url"].toString();
        course.extra = grabExtra(map);
        //course.video = map["teaser_video"].toMap()["youtube_url"].toString();

        QList<QVariant> instructors = map["visible_instructors"].toList();
        for (const QVariant& j : instructors)
        {
            QVariantMap imap = j.toMap();
            Instructor instr;
            instr.image = imap["image_100x100"].toString();
            instr.bio = imap["job_title"].toString();
            instr.name = imap["title"].toString();

            course.instructors.append(instr);
        }

        QVariantMap kmap = map["primary_category"].toMap();
        course.departments.append(kmap["title"].toString());

        //qCDebug(Udemy) << "Category count: " << course.departments.size() << course.departments;
        //qCDebug(Udemy) << "Instr count: " << course.instructors.size();
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

const QMap<QByteArray, QByteArray> UdemyClient::customHeaders() const
{
    QMap<QByteArray, QByteArray> res;
    res.insert("Authorization", "Basic MlloUmZ1TXpUSjJLMjJmZWZoSldTeVoyanVtOWx0dkdoWFhFUWZQaTpiNGRIUXhmUDdsODVWa3RHQlM4dUFpdU5ZclpyOEZWY3E3cFpTaWRXbVNMSTBuNm5mWGFyRUxSQ2xqdEtDbjZPcTR3ZkZwWjlqM0RsdU13aUhDN0UxVW1zS1YyQzRtSUlvR2ZEYXpNYVhtbDZjRGtHcjJmOHVqVzVkQ2J5VThaaw==");
    return res;
}

QString UdemyClient::grabExtra(const QVariantMap &map)
{
    return _("price - ") + map["price"].toString();
    //    QStringList extra;
    //    extra << QStringLiteral("price - ") + map["price"].toString();
    //    return extra.join(", ");
}
