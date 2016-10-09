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

#include <courseraclient.h>
#include <localization.h>

#include <core/net/error.h>
#include <core/net/http/client.h>
#include <core/net/http/content_type.h>
#include <core/net/http/response.h>
#include <QVariantMap>
#include <json/json.h>

Q_LOGGING_CATEGORY(Coursera, "Coursera")

namespace http = core::net::http;
namespace net = core::net;
namespace json = Json;

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
    params.push_back({"fields", "instructors.v1(firstName,lastName,suffix,photo,photo150,bio),description,photoUrl,slug,workload,instructorIds,domainTypes" });
    if (!query.isEmpty())
    {
        params.push_back({"q", "search"});
        params.push_back({"query", query.toStdString()});
        setCacheEnabled(false);
    }

    qCDebug(Coursera) << "Download started...";
    get(path, params, data);
    qCDebug(Coursera) << "Data received:" << data.length() << "bytes";

    json::Value root;
    json::Reader reader;
    reader.parse(data.data(), root); // TODO BUG

    QMap<string, Instructor> instructorsMap;

    json::Value instrs = root["linked"]["instructors.v1"];

    for (json::ArrayIndex index = 0; index < list.size(); ++index)
    {
        json::Value imap = instrs.get(index, json::Value());

        Instructor instr;
        instr.image = imap["photo"].asString();
        instr.bio = imap["bio"].asString();
        instr.name = imap["firstName"].asString() + " " + imap["lastName"].asString();
        instructorsMap.insert(imap["id"].asString(), instr);
    }

    json::Value elems = root["elements"];
    qCDebug(Coursera) << "Element count:" << elems.size();

    for (json::ArrayIndex index = 0; index < elems.size(); ++index)
    {
        json::Value map = elems.get(index, json::Value());

        Course course;
        course.id = map["id"].asString();
        course.slug = map["slug"].asString();
        course.title = map["name"].asString();
        course.description = map["description"].asString();
        course.headline = course.description.substr(0, 120) + ("...");
        course.art = map["photoUrl"].asString();
        course.link = ("http://www.coursera.org/learn/") + map["slug"].asString();
        //course.extra = grabExtra(map); // TODO

        json::Value instructorsIds = map["instructorIds"];
        for (json::ArrayIndex jj = 0; jj < instructorsIds.size(); ++jj)
        {
            json::Value jmap = instructorsIds.get(jj, json::Value());
            if (instructorsMap.contains(jmap.asString()))
                course.instructors.push_back(instructorsMap.value(jmap.asString()));
        }

        json::Value domainTypes = map["domainTypes"];
        for (json::ArrayIndex kk = 0; kk < domainTypes.size(); ++kk)
        {
            json::Value kmap = domainTypes.get(kk, json::Value());
            course.departments.push_back(kmap["domainId"].asString());
        }

        //qCDebug(Coursera) << "Domain count: " << course.departments;
        //qCDebug(Coursera) << "Instr count: " << course.instructors.size();
        list.append(course);
    }

    // ----------------------------------------------------------------- //


    //QJsonDocument root = QJsonDocument::fromJson(data);

//    QVariantMap variant = root.toVariant().toMap();

//    // Instructors.

//    QList<QVariant> instrs = variant["linked"].toMap()["instructors.v1"].toList();
//    //qCDebug(Coursera) << "Instructor count:" << instrs.length();

//    QMap<QString, Instructor> instructorsMap;

//    for (const QVariant &j : instrs)
//    {
//        QVariantMap imap = j.toMap();
//        Instructor instr;
//        instr.image = imap["photo"].toString();
//        instr.bio = imap["bio"].toString();
//        instr.name = imap["firstName"].toString() + " " + imap["lastName"].toString();

//        //qCDebug(Coursera) << "Instr details:" << instr.image << instr.name;

//        instructorsMap.insert(imap["id"].toString(), instr);
//    }

//    // Courses.

//    QList<QVariant> elems = variant["elements"].toList();
//    qCDebug(Coursera) << "Element count:" << elems.length();

//    for (const QVariant &i : elems)
//    {
//        QVariantMap map = i.toMap();

//        Course course;
//        course.id = map["id"].toString();
//        course.slug = map["slug"].toString();
//        course.title = map["name"].toString();
//        course.description = map["description"].toString();
//        course.headline = course.description.left(120) + QStringLiteral("...");
//        course.art = map["photoUrl"].toString();
//        course.link = QStringLiteral("http://www.coursera.org/learn/") + map["slug"].toString();
//        course.extra = grabExtra(map);

//        QList<QVariant> instructorsIds = map["instructorIds"].toList();
//        for (const QVariant& j : instructorsIds)
//        {
//            if (instructorsMap.contains(j.toString()))
//                course.instructors.append(instructorsMap.value(j.toString()));
//        }

//        QList<QVariant> domainTypes = map["domainTypes"].toList();
//        for (const QVariant& k : domainTypes)
//        {
//            QVariantMap kmap = k.toMap();
//            course.departments.append(kmap["domainId"].toString());
//        }

//        //qCDebug(Coursera) << "Domain count: " << course.departments;
//        //qCDebug(Coursera) << "Instr count: " << course.instructors.size();
//        list.append(course);
//    }

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

QString CourseraClient::grabExtra(const QVariantMap &map)
{
    return _("workload - ") + map["workload"].toString();
}

