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

#include <iversityclient.h>
#include <localization.h>

#include <core/net/error.h>
#include <core/net/http/client.h>
#include <core/net/http/content_type.h>
#include <core/net/http/response.h>
#include <QVariantMap>
#include <json/json.h>

Q_LOGGING_CATEGORY(Iversity, "Iversity")

namespace http = core::net::http;
namespace net = core::net;
namespace json = Json;

using namespace std;

IversityClient::IversityClient(Config::Ptr config) :
    CachedClient(config)
{ }

vector<Course> IversityClient::courses(const string &query)
{
    vector<Course> list;

    SearchEngine se(query);

    QByteArray data;
    net::Uri::Path path;
    net::Uri::QueryParameters params;

    qCDebug(Iversity) << "Download started...";
    get( path, params, data);
    qCDebug(Iversity) << "Data received:" << data.length() << "bytes";

    json::Value root;
    json::Reader reader;
    reader.parse(data.data(), root); // TODO

    json::Value courses = root["courses"];
    qCDebug(Iversity) << "Element count:" << courses.size();


    for (json::ArrayIndex index = 0; index < courses.size(); ++index)
    {
        json::Value map = courses.get(index, json::Value());

        Course course;
        course.id = map["id"].asString();
        course.slug = map["url"].asString();
        course.title = map["title"].asString();
        course.description = map["description"].asString();
        course.headline = map["subtitle"].asString();
        course.art = map["cover"].asString();
        course.link = map["url"].asString();
        course.extra = _("level - ") + map["knowledge_level"].asString();
        course.video = map["trailer_video"].asString();

        json::Value instructors = map["instructors"];
        for (json::ArrayIndex jj = 0; jj < instructors.size(); ++jj)
        {
            json::Value imap = instructors.get(jj, json::Value());
            Instructor instr;
            instr.image = imap["image"].asString();
            instr.bio = imap["biography"].asString();
            instr.name = imap["name"].asString();

            course.instructors.push_back(instr);
        }

        course.departments.push_back(map["discipline"].asString());

        //qCDebug(Iversity) << "Category count: " << course.departments;
        //qCDebug(Iversity) << "Instr count: " << course.instructors.size();
        if (query.empty() || se.isMatch(course))
            list.push_back(course);
    }

    return list;
}

const string IversityClient::baseApiUrl() const
{
    return ("https://iversity.org/api/v1/courses");
}

const string IversityClient::name() const
{
    return ("iversity");
}
