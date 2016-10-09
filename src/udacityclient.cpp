/*
 * Copyright (C) 2016 Roman Shchekin aka mrqtros.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <udacityclient.h>
#include <localization.h>

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
    CachedClient(config)
{ }

QList<Course> UdacityClient::courses(const QString &query)
{
    QList<Course> list;

//    QByteArray data;
//    net::Uri::Path path;
//    net::Uri::QueryParameters params;

//    qCDebug(Udacity) << "Download started...";
//    get( path, params, data);
//    qCDebug(Udacity) << "Data received:" << data.length() << "bytes";
//    QJsonDocument root = QJsonDocument::fromJson(data);

//    QVariantMap variant = root.toVariant().toMap();
//    QList<QVariant> courses = variant["courses"].toList();
//    qCDebug(Udacity) << "Element count:" << courses.length();

//    SearchEngine se(query);

//    for (const QVariant &i : courses)
//    {
//        QVariantMap map = i.toMap();

//        Course course;
//        course.id = map["key"].toString();
//        course.slug = map["slug"].toString();
//        course.title = map["title"].toString();
//        course.description = map["summary"].toString();
//        course.headline = map["short_summary"].toString();
//        course.art = map["image"].toString();
//        course.link = map["homepage"].toString();
//        course.video = map["teaser_video"].toMap()["youtube_url"].toString();
//        course.extra = grabExtra(map);
//        //qCDebug(Udacity) << "VIDEO URL" << course.video;

//        QList<QVariant> instructors = map["instructors"].toList();
//        for (const QVariant& j : instructors)
//        {
//            QVariantMap imap = j.toMap();
//            Instructor instr;
//            instr.image = imap["image"].toString();
//            instr.bio = imap["bio"].toString();
//            instr.name = imap["name"].toString();

//            course.instructors.append(instr);
//        }

//        QList<QVariant> tracks = map["tracks"].toList();
//        for (const QVariant& k : tracks)
//            course.departments.append(k.toString());

//        //qCDebug(Udacity) << "Track count: " << course.departments;
//        //qCDebug(Udacity) << "Instr count: " << course.instructors.size();
//        if (query.isEmpty() || se.isMatch(course))
//            list.append(course);
//    }

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

QString UdacityClient::grabExtra(const QVariantMap &map)
{
    QStringList extra;
    extra << _("level - ") + map["level"].toString();
    extra << _("duration - ") + map["expected_duration"].toString() + " " + map["expected_duration_unit"].toString();
    return extra.join(", ");
}
