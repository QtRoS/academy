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

#include <udemyclient.h>
#include <localization.h>

#include <core/net/error.h>
#include <core/net/http/client.h>
#include <core/net/http/content_type.h>
#include <core/net/http/response.h>
#include <QVariantMap>
#include <json/json.h>

Q_LOGGING_CATEGORY(Udemy, "Udemy")

namespace http = core::net::http;
namespace net = core::net;
namespace json = Json;

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
    get(path, params, data);
    qCDebug(Udemy) << "Data received:" << data.length() << "bytes";

    json::Value root;
    json::Reader reader;
    reader.parse(data.data(), root); // TODO

    json::Value courses = root["results"];
    qCDebug(Udemy) << "Element count:" << courses.size();

    for (json::ArrayIndex index = 0; index < courses.size(); ++index)
    {
        json::Value map = courses.get(index, json::Value());

        Course course;
        course.id = map["id"].asString();
        course.slug = map["url"].asString();
        course.title = map["title"].asString();
        course.description = map["description"].asString();
        course.headline = map["headline"].asString();
        course.art = map["image_480x270"].asString();
        course.link = ("https://www.udemy.com") + map["url"].asString();
        //course.extra = grabExtra(map);
        //course.video = map["teaser_video"].toMap()["youtube_url"].toString();

        json::Value instructors = map["visible_instructors"];
        for (json::ArrayIndex jj = 0; jj < instructors.size(); ++jj)
        {
            json::Value imap = instructors.get(jj, json::Value());
            Instructor instr;
            instr.image = imap["image_100x100"].asString();
            instr.bio = imap["job_title"].asString();
            instr.name = imap["title"].asString();

            course.instructors.push_back(instr);
        }

        json::Value kmap = map["primary_category"];
        course.departments.push_back(kmap["title"].asString());

        //qCDebug(Udemy) << "Category count: " << course.departments;
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
