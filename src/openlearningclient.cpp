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

#include <openlearningclient.h>
#include <localization.h>

#include <core/net/error.h>
#include <core/net/http/client.h>
#include <core/net/http/content_type.h>
#include <core/net/http/response.h>
#include <json/json.h>

Q_LOGGING_CATEGORY(OpenLearning, "OpenLearning")

namespace http = core::net::http;
namespace net = core::net;
namespace json = Json;

using namespace std;

OpenLearningClient::OpenLearningClient(Config::Ptr config) :
    CachedClient(config)
{ }

vector<Course> OpenLearningClient::courses(const string &query)
{
    vector<Course> list;

    SearchEngine se(query);

    QByteArray data;
    net::Uri::Path path;
    net::Uri::QueryParameters params;

    qCDebug(OpenLearning) << "Download started...";
    get( path, params, data);
    qCDebug(OpenLearning) << "Data received:" << data.length() << "bytes";

    json::Value root;
    json::Reader reader;
    reader.parse(data.data(), root); // TODO

    json::Value courses = root["courses"];

    for (json::ArrayIndex index = 0; index < courses.size(); ++index)
    {
        json::Value map = courses.get(index, json::Value());

        Course course;
        course.id = map["courseUrl"].asString();
        course.slug = map["courseUrl"].asString();
        course.title = map["name"].asString();
        course.description = map["summary"].asString();
        course.headline = course.description.length() > 120 ? course.description.substr(120) + ("...") : course.description;
        course.art = map["image"].asString();
        course.link = map["courseUrl"].asString();
        //course.extra = grabExtra(map);
        course.video = grabVideo(map["promoMediaUrl"].asString());

        {
            json::Value imap = map["creator"];
            Instructor instr;
            instr.image = imap["imageUrl"].asString();
            instr.bio = imap["profileUrl"].asString();
            instr.name = imap["fullName"].asString();
            course.instructors.push_back(instr);
        }

        course.departments.push_back(map["category"].asString());

        //qCDebug(OpenLearning) << "Categories: " << course.departments;
        //qCDebug(OpenLearning) << "Instr count: " << course.instructors.size();
        if (query.empty() || se.isMatch(course))
            list.push_back(course);
    }

    return list;
}

const string OpenLearningClient::baseApiUrl() const
{
    return ("https://www.openlearning.com/api/courses/list?type=free,paid");
}

const string OpenLearningClient::name() const
{
    return ("OpenLearning");
}

QString OpenLearningClient::grabExtra(const QVariantMap &map)
{
    return _("duration - ") + map["duration"].toString();
}

string OpenLearningClient::grabVideo(const string &promo)
{
    if (promo.find("youtube") != std::string::npos || promo.find("vimeo") != std::string::npos)
        return promo;
    else return string();
}
