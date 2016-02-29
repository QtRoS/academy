#include <iversityclient.h>
#include <localization.h>

#include <core/net/error.h>
#include <core/net/http/client.h>
#include <core/net/http/content_type.h>
#include <core/net/http/response.h>
#include <QVariantMap>

Q_LOGGING_CATEGORY(Iversity, "Iversity")

namespace http = core::net::http;
namespace net = core::net;

using namespace std;

IversityClient::IversityClient(Config::Ptr config) :
    CachedClient(config)
{ }

QList<Course> IversityClient::courses(const QString &query)
{
    QList<Course> list;

    QByteArray data;
    net::Uri::Path path;
    net::Uri::QueryParameters params;

    qCDebug(Iversity) << "Download started...";
    if (data.isNull())
        get( path, params, data);
    qCDebug(Iversity) << "Data received:" << data.length() << "bytes";
    QJsonDocument root = QJsonDocument::fromJson(data);

    QVariantMap variant = root.toVariant().toMap();
    QList<QVariant> courses = variant["courses"].toList();
    qCDebug(Iversity) << "Element count:" << courses.length();

    SearchEngine se(query);

    for (const QVariant &i : courses)
    {
        QVariantMap map = i.toMap();

        Course course;
        course.id = map["id"].toString();
        course.slug = map["url"].toString();
        course.title = map["title"].toString();
        course.description = map["description"].toString();
        course.headline = map["subtitle"].toString();
        course.art = map["cover"].toString();
        course.link = map["url"].toString();
        course.extra = grabExtra(map);
        course.video = map["trailer_video"].toString();

        QList<QVariant> instructors = map["instructors"].toList();
        for (const QVariant& j : instructors)
        {
            QVariantMap imap = j.toMap();
            Instructor instr;
            instr.image = imap["image"].toString();
            instr.bio = imap["biography"].toString();
            instr.name = imap["name"].toString();

            course.instructors.append(instr);
        }

        course.departments.append(map["discipline"].toString());

        //qCDebug(Iversity) << "Category count: " << course.departments;
        //qCDebug(Iversity) << "Instr count: " << course.instructors.size();
        if (query.isEmpty() || se.isMatch(course))
            list.append(course);
    }

    return list;
}

const QString IversityClient::baseApiUrl() const
{
    return QStringLiteral("https://iversity.org/api/v1/courses");
}

const QString IversityClient::name() const
{
    return QStringLiteral("iversity");
}

QString IversityClient::grabExtra(const QVariantMap &map)
{
    return _("level - ") + map["knowledge_level"].toString();
    //    QStringList extra;
    //    extra << QStringLiteral("price - ") + map["price"].toString();
    //    return extra.join(", ");
}
