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
#include <json/json.h>

namespace json = Json;

Q_LOGGING_CATEGORY(Coursera, "Coursera")

namespace http = core::net::http;
namespace net = core::net;

using namespace std;

CourseraClient::CourseraClient(Config::Ptr config) :
    CachedClient(config)
{ }

vector<Course> CourseraClient::courses(const string &query)
{
    vector<Course> list;

    net::Uri::Path path;
    net::Uri::QueryParameters params;
    QByteArray data;

    params.push_back({"includes", "instructorIds"});
    params.push_back({"limit", "80"});
    params.push_back({"fields", "instructors.v1(firstName,lastName,suffix,photo,photo150,bio),description,photoUrl,slug,workload,instructorIds,domainTypes" });
    if (!query.empty())
    {
        params.push_back({"q", "search"});
        params.push_back({"query", query});
        setCacheEnabled(false);
    }

    qCDebug(Coursera) << "Download started...";
    get(path, params, data);
    qCDebug(Coursera) << "Data received:" << data.length() << "bytes";

    json::Value root;
    json::Reader reader;
    reader.parse(data.data(), root);

    QMap<string, Instructor> instructorsMap;

    json::Value instrs = root["linked"]["instructors.v1"];
    qCDebug(Coursera) << "Instrs count:" << instrs.size();

    for (json::ArrayIndex index = 0; index < instrs.size(); ++index)
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
        course.extra = _("workload - ") + map["workload"].asString();

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
        list.push_back(course);
    }

    return list;
}

const string CourseraClient::baseApiUrl() const
{
    return ("https://api.coursera.org/api/courses.v1");
}

const string CourseraClient::name() const
{
    return ("Coursera");
}
