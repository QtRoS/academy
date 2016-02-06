#include <courseraclient.h>

#include <core/net/error.h>
#include <core/net/http/client.h>
#include <core/net/http/content_type.h>
#include <core/net/http/response.h>
#include <QVariantMap>

Q_LOGGING_CATEGORY(Coursera, "Coursera")

namespace http = core::net::http;
namespace net = core::net;

using namespace std;

CourseraClient::CourseraClient(Config::Ptr config) :
    CachedClient(config)
{ }

QList<Course> CourseraClient::courses(const QString &query)
{
    QList<Course> list;

    QByteArray data;
    net::Uri::Path path;
    net::Uri::QueryParameters params;
    params.push_back({"includes", "instructorIds"});
    params.push_back({"limit", "80"});
    params.push_back({"fields", "instructors.v1(firstName,lastName,suffix,photo,photo150,bio),language,description,photoUrl,slug,instructorIds" }); //"language,description,photoUrl,slug"
    if (!query.isEmpty())
    {
        params.push_back({"q", "search"});
        params.push_back({"query", query.toStdString()});
        setCacheEnabled(false);
    }

    qCDebug(Coursera) << "Download started...";
    get( path, params, data);
    qCDebug(Coursera) << "Data received:" << data.length() << "bytes";
    QJsonDocument root = QJsonDocument::fromJson(data);

    QVariantMap variant = root.toVariant().toMap();

    // Instructors.

    QList<QVariant> instrs = variant["linked"].toMap()["instructors.v1"].toList();
    qCDebug(Coursera) << "Instructor count:" << instrs.length();

    QMap<QString, Instructor> instructorsMap;

    for (const QVariant &j : instrs)
    {
        QVariantMap imap = j.toMap();
        Instructor instr;
        instr.image = imap["photo"].toString();
        instr.bio = imap["bio"].toString();
        instr.name = imap["firstName"].toString() + " " + imap["lastName"].toString();

        //qCDebug(Coursera) << "Instr details:" << instr.image << instr.name;

        instructorsMap.insert(imap["id"].toString(), instr);
    }

    // Courses.

    QList<QVariant> elems = variant["elements"].toList();
    qCDebug(Coursera) << "Element count:" << elems.length();

    for (const QVariant &i : elems)
    {
        QVariantMap map = i.toMap();

        Course course;
        course.id = map["id"].toString();
        course.slug = map["slug"].toString();
        course.title = map["name"].toString();
        course.description = map["description"].toString();
        course.shortDescription = course.description.left(120) + QStringLiteral("...");
        course.art = map["photoUrl"].toString();
        course.link = QStringLiteral("http://www.coursera.org/learn/") + map["slug"].toString();

        QList<QVariant> instructorsIds = map["instructorIds"].toList();
        for (const QVariant& j : instructorsIds)
        {
            if (instructorsMap.contains(j.toString()))
                course.instructors.append(instructorsMap.value(j.toString()));
        }

        //qCDebug(Coursera) << "Instr count: " << course.instructors.size();
        list.append(course);
    }

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

