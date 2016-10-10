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

#include <query.h>
#include <localization.h>
#include <scopeimagecache.h>
#include <thesuffering.h>

#include <unity/scopes/Annotation.h>
#include <unity/scopes/CategorisedResult.h>
#include <unity/scopes/CategoryRenderer.h>
#include <unity/scopes/QueryBase.h>
#include <unity/scopes/SearchReply.h>

#include <vector>
#include <string>
#include <locale>
#include <set>

Q_LOGGING_CATEGORY(Qry, "Query")

namespace sc = unity::scopes;

using namespace std;


const static string CURRENT_TEMPLATE_OV =
        R"(
{
        "schema-version": 1,
        "template": {
        "category-layout": "grid",
        "card-size": "medium",
        "overlay": true
        },
        "components": {
        "title": "title",
        "art" : {
        "field": "art"
        }
        }
        }
        )";


const static string CURRENT_TEMPLATE =
        R"(
{
        "schema-version": 1,
        "template": {
        "category-layout": "grid",
        "card-size": "medium"
        },
        "components": {
        "title": "title",
        "art" : {
        "field": "art"
        }
        }
        }
        )";

Query::Query(const sc::CannedQuery &query, const sc::SearchMetadata &metadata, Config::Ptr config) :
    sc::SearchQueryBase(query, metadata),
    //client_(config),
    m_config(config),
    m_coursera(config),
    m_udemy(config),
    m_edx(config),
    m_udacity(config),
    m_iversity(config),
    m_openLearning(config)
{

}

void Query::cancelled()
{
    qCDebug(Qry) << "Cancelled";
    //client_.cancel();
    m_coursera.cancel();
    m_udemy.cancel();
    m_edx.cancel();
    m_udacity.cancel();
    m_iversity.cancel();
    m_openLearning.cancel();
}


void Query::run(sc::SearchReplyProxy const& reply)
{
    try {
        // Start by getting information about the query
        const sc::CannedQuery &query(sc::SearchQueryBase::query());

        static ScopeImageCache* icache = nullptr;
        if (!icache)
            icache = new ScopeImageCache();

        // Get query string and selected department.
        string queryString = query.query_string();
        qCDebug(Qry) << "Query string is:" << QString::fromStdString(queryString);
        string selectedDepartment = query.department_id();
        //qCDebug(Qry) << "Selected department:" << selectedDepartment;

        // Getting source list.
        vector<BaseClient*> sources = enabledSources();
        qCDebug(Qry) << "Source count:" << sources.size();

        // Create and register departments.
        sc::Department::SPtr allDepts = sc::Department::create("", query, _("All"));
        sc::DepartmentList depList;
        vector<Department> list = DepartmentManager::departments();
        for (int i = 0; i < list.size(); i++)
            depList.push_back(sc::Department::create(list[i].id, query, list[i].label));
        allDepts->set_subdepartments(depList);
        reply->register_departments(allDepts);

        set<string> uniqueSet;
        for(int i = 0; i < sources.size(); ++i)
        {
            string sourceCatName = sources[i]->name();
            auto courseList = sources[i]->courses(queryString);

            auto templ = settings().at("textPosition").get_int() ? CURRENT_TEMPLATE_OV : CURRENT_TEMPLATE;
            auto sourceCategory = reply->register_category(sourceCatName, sourceCatName, "", sc::CategoryRenderer(templ));

            //qCDebug(Qry) << "Processing source:" << sourceCatName;
            int maxCacheSize = 8;
            int cacheUsage = 0;

            for (const auto &course : courseList)
            {
                // Department check.
                if (!selectedDepartment.empty() && !DepartmentManager::isMatch(course, selectedDepartment))
                    continue;

                // Duplicate results cause Dash to crash.
                if (uniqueSet.count(course.link))
                    continue;
                uniqueSet.insert(course.link);

                string art = course.art;
                if (++cacheUsage <= maxCacheSize)
                {
                    string cachedArt = icache->getCached(course.art);
                    qCDebug(Qry) << "cachedArt" << cachedArt.c_str();
                    if (!cachedArt.empty())
                        art = cachedArt;
                }

                sc::CategorisedResult res(sourceCategory);
                res.set_uri(course.link);
                res.set_title(course.title);
                res.set_art(art);
                res["headline"] = course.headline;
                res["description"] = course.description;
                res["source"] = sourceCatName;
                res["extra"] = course.extra;
                if (!course.video.empty())
                    res["video_url"] = course.video;
                res["departments"] = utils::join(course.departments, ";");

                // Add instructors to map.
                if (course.instructors.size() > 0)
                {
                    std::vector<sc::Variant> images, names, bios;
                    for (auto j = course.instructors.begin(); j != course.instructors.end(); ++j)
                    {
                        images.push_back(sc::Variant(j->image));
                        names.push_back(sc::Variant(j->name));
                        bios.push_back(sc::Variant((j->bio.length() < 150 ? j->bio : j->bio.substr(0, 150) + "...")));
                    }
                    res["instr_images"] = images;
                    res["instr_names"] = names;
                    res["instr_bios"] = bios;
                }

                // Push the result
                if (!reply->push(res))
                    return;
            }

            qCDebug(Qry) << "Finished with source:" << QString::fromStdString(sourceCatName);
        }

    } catch (domain_error &e) {
        qCDebug(Qry) << "Exception:" << QString::fromStdString(e.what());
        reply->error(current_exception());
    }
}

vector<BaseClient *> Query::enabledSources()
{
    vector<BaseClient*> sources;

    // Checking out which sources are enabled.
    if (settings().at("coursera").get_bool())
        sources.push_back(&m_coursera);
    if (settings().at("udemy").get_bool())
        sources.push_back(&m_udemy);
    if (settings().at("udacity").get_bool())
        sources.push_back(&m_udacity);
    if (settings().at("iversity").get_bool())
        sources.push_back(&m_iversity);
    if (settings().at("openLearning").get_bool())
        sources.push_back(&m_openLearning);
    if (settings().at("edx").get_bool())
        sources.push_back(&m_edx);

    return sources;
}

