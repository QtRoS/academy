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
#include <json/json.h>

Q_LOGGING_CATEGORY(Udacity, "Udacity")

namespace http = core::net::http;
namespace net = core::net;
namespace json = Json;

using namespace std;

UdacityClient::UdacityClient(Config::Ptr config) :
    CachedClient(config)
{ }

vector<Course> UdacityClient::courses(const string &query)
{
    vector<Course> list;

    SearchEngine se(query);

    QByteArray data;
    net::Uri::Path path;
    net::Uri::QueryParameters params;

    qCDebug(Udacity) << "Download started...";
    get( path, params, data);
    qCDebug(Udacity) << "Data received:" << data.length() << "bytes";

    json::Value root;
    json::Reader reader;
    reader.parse(data.data(), root); // TODO

    json::Value courses = root["courses"];
    qCDebug(Udacity) << "Element count:" << courses.size();


    for (json::ArrayIndex index = 0; index < courses.size(); ++index)
    {
        json::Value map = courses.get(index, json::Value());

        Course course;
        course.id = map["key"].asString();
        course.slug = map["slug"].asString();
        course.title = map["title"].asString();
        course.description = map["summary"].asString();
        course.headline = map["short_summary"].asString();
        course.art = map["image"].asString();
        course.link = map["homepage"].asString();
        course.video = map["teaser_video"]["youtube_url"].asString();
        course.extra =  _("level - ") + map["level"].asString() +
                        _("duration - ") + map["expected_duration"].asString() + " " + map["expected_duration_unit"].asString();
        //qCDebug(Udacity) << "VIDEO URL" << course.video;

        json::Value instructors = map["instructors"];
        for (json::ArrayIndex jj = 0; jj < instructors.size(); ++jj)
        {
            json::Value imap = instructors.get(jj, json::Value());
            Instructor instr;
            instr.image = imap["image"].asString();
            instr.bio = imap["bio"].asString();
            instr.name = imap["name"].asString();

            course.instructors.push_back(instr);
        }

        json::Value tracks = map["tracks"];
        for (json::ArrayIndex kk = 0; kk < instructors.size(); ++kk)
        {
            json::Value kmap = tracks.get(kk, json::Value());
            course.departments.push_back(kmap.asString());
        }
        //qCDebug(Udacity) << "Track count: " << course.departments;
        //qCDebug(Udacity) << "Instr count: " << course.instructors.size();
        if (query.empty() || se.isMatch(course))
            list.push_back(course);
    }

    return list;
}

const string UdacityClient::baseApiUrl() const
{
    return ("https://www.udacity.com/public-api/v0/courses");
}

const string UdacityClient::name() const
{
    return ("Udacity");
}
