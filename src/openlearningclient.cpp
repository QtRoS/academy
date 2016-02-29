#include <openlearningclient.h>
#include <localization.h>

#include <core/net/error.h>
#include <core/net/http/client.h>
#include <core/net/http/content_type.h>
#include <core/net/http/response.h>
#include <QVariantMap>

Q_LOGGING_CATEGORY(OpenLearning, "OpenLearning")

namespace http = core::net::http;
namespace net = core::net;

using namespace std;

OpenLearningClient::OpenLearningClient(Config::Ptr config) :
    CachedClient(config)
{ }

QList<Course> OpenLearningClient::courses(const QString &query)
{
    QList<Course> list;

    QByteArray data;
    net::Uri::Path path;
    net::Uri::QueryParameters params;

    qCDebug(OpenLearning) << "Download started...";
    if (data.isNull())
        get( path, params, data);
    qCDebug(OpenLearning) << "Data received:" << data.length() << "bytes";
    QJsonDocument root = QJsonDocument::fromJson(data);

    QVariantMap variant = root.toVariant().toMap();
    QList<QVariant> courses = variant["courses"].toList();
    qCDebug(OpenLearning) << "Element count:" << courses.length();

    SearchEngine se(query);

    for (const QVariant &i : courses)
    {
        QVariantMap map = i.toMap();

        Course course;
        course.id = map["courseUrl"].toString();
        course.slug = map["courseUrl"].toString();
        course.title = map["name"].toString();
        course.description = map["summary"].toString();
        course.headline = course.description.left(120) + QStringLiteral("...");
        course.art = map["image"].toString();
        course.link = map["courseUrl"].toString();
        course.extra = grabExtra(map);
        course.video = grabVideo(map["promoMediaUrl"].toString());

        {
            QVariantMap imap = map["creator"].toMap();
            Instructor instr;
            instr.image = imap["imageUrl"].toString();
            instr.bio = imap["profileUrl"].toString();
            instr.name = imap["fullName"].toString();
            course.instructors.append(instr);
        }

        course.departments.append(map["category"].toString());

        qCDebug(OpenLearning) << "Categories: " << course.departments;
        //qCDebug(OpenLearning) << "Instr count: " << course.instructors.size();
        if (query.isEmpty() || se.isMatch(course))
            list.append(course);
    }

    return list;
}

const QString OpenLearningClient::baseApiUrl() const
{
    return QStringLiteral("https://www.openlearning.com/api/courses/list?type=free,paid");
}

const QString OpenLearningClient::name() const
{
    return QStringLiteral("OpenLearning");
}

QString OpenLearningClient::grabExtra(const QVariantMap &map)
{
    return _("duration - ") + map["duration"].toString();
    //    QStringList extra;
    //    extra << QStringLiteral("price - ") + map["price"].toString();
    //    return extra.join(", ");
}

QString OpenLearningClient::grabVideo(const QString &promo)
{
    if (promo.contains("youtube") || promo.contains("vimeo"))
        return promo;
    else return QString();
}
