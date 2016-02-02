#include <query.h>
#include <localization.h>

#include <unity/scopes/Annotation.h>
#include <unity/scopes/CategorisedResult.h>
#include <unity/scopes/CategoryRenderer.h>
#include <unity/scopes/QueryBase.h>
#include <unity/scopes/SearchReply.h>

#include <iomanip>
#include <iostream>
#include <sstream>

namespace sc = unity::scopes;

using namespace std;


/**
 * Define the larger "current weather" layout.
 *
 * The icons are larger.
 */
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
        },
        "subtitle": "subtitle"
        }
        }
        )";

/**
 * Define the layout for the forecast results.
 *
 * The icon size is small, and ask for the card layout
 * itself to be horizontal. I.e. the text will be placed
 * next to the image.
 */
const static string FORECAST_TEMPLATE =
        R"(
{
        "schema-version": 1,
        "template": {
        "category-layout": "grid",
        "card-layout": "horizontal",
        "card-size": "small"
        },
        "components": {
        "title": "title",
        "art" : {
        "field": "art"
        },
        "subtitle": "subtitle"
        }
        }
        )";

Query::Query(const sc::CannedQuery &query, const sc::SearchMetadata &metadata, Config::Ptr config) :
    sc::SearchQueryBase(query, metadata), client_(config), m_config(config)
{

}

void Query::cancelled()
{
    client_.cancel();
}


void Query::run(sc::SearchReplyProxy const& reply)
{
    try {
        // Start by getting information about the query
        const sc::CannedQuery &query(sc::SearchQueryBase::query());

        // Get the query string
        string query_string = query.query_string();

        QList<QSharedPointer<BaseClient>> sources;

        if (settings().at("coursera").get_bool())
            sources.append(QSharedPointer<BaseClient>(new CourseraClient(m_config)));
        if (settings().at("edx").get_bool())
            sources.append(QSharedPointer<BaseClient>(new EdxClient(m_config)));
        if (settings().at("udacity").get_bool())
            sources.append(QSharedPointer<BaseClient>(new UdacityClient(m_config)));
        if (settings().at("udemy").get_bool())
            sources.append(QSharedPointer<BaseClient>(new UdemyClient(m_config)));

        for(int i = 0; i < sources.count(); i++)
        {
            QString sourceCatName = sources[i]->name();
            auto sourceList = sources[i]->courses(QString::fromStdString(query_string));

            auto sourceCategory = reply->register_category(sourceCatName.toLower().toStdString(),
                                                             sourceCatName.toStdString(),
                                                             "", sc::CategoryRenderer(CURRENT_TEMPLATE));

            for (const auto &course : sourceList)
            {
                sc::CategorisedResult res(sourceCategory);

                // We must have a URI
                res.set_uri(course.link.toStdString());
                res.set_title(course.title.toStdString());
                res.set_art(course.art.toStdString());
                res["subtitle"] = course.subTitle.toStdString();
                res["description"] = course.description.toStdString();
                if (!course.video.isEmpty())
                    res["video_url"] = course.video.toStdString();

                // Add instructors to map.
                if (course.instructors.count() > 0)
                {
                    std::vector<sc::Variant> images, names, bios;
                    for (auto j = course.instructors.begin(); j != course.instructors.end(); ++j)
                    {
                        images.push_back(sc::Variant(j->image.toStdString()));
                        names.push_back(sc::Variant(j->name.toStdString()));
                        bios.push_back(sc::Variant((j->bio.length() < 150 ? j->bio : j->bio.left(150) + "...").toStdString()));
                    }
                    res["instr_images"] = images;
                    res["instr_names"] = names;
                    res["instr_bios"] = bios;
                }

                // Push the result
                if (!reply->push(res))
                    return;
            }
        }


//        // ----------------------- Coursera

//        CourseraClient coursera(m_config);
//        QString courseraCatName = coursera.name();
//        auto courseraList = coursera.courses(QString::fromStdString(query_string));

//        // Register a category for the current weather, with the title we just built
//        auto courseraCategory = reply->register_category(courseraCatName.toLower().toStdString(),
//                                                         courseraCatName.toStdString(),
//                                                         "", sc::CategoryRenderer(CURRENT_TEMPLATE));

//        for (const auto &course : courseraList)
//        {
//            // Create a single result for the current weather category
//            sc::CategorisedResult res(courseraCategory);

//            // We must have a URI
//            res.set_uri(course.link.toStdString());
//            res.set_title(course.title.toStdString());
//            res.set_art(course.art.toStdString());
//            res["subtitle"] = course.subTitle.toStdString();
//            res["description"] = course.description.toStdString();
//            if (!course.video.isEmpty())
//                res["video_url"] = course.video.toStdString();

//            // Push the result
//            if (!reply->push(res))
//                return;
//        }

//        // ----------------------- Udemy

//        UdemyClient udemy(m_config);
//        QString udemyCatName = udemy.name();
//        auto udemyList = udemy.courses(QString::fromStdString(query_string));

//        // Register a category for the current weather, with the title we just built
//        auto udemyCategory = reply->register_category(udemyCatName.toLower().toStdString(),
//                                                         udemyCatName.toStdString(),
//                                                         "", sc::CategoryRenderer(CURRENT_TEMPLATE));

//        for (const auto &course : udemyList)
//        {
//            // Create a single result for the current weather category
//            sc::CategorisedResult res(udemyCategory);

//            // We must have a URI
//            res.set_uri(course.link.toStdString());
//            res.set_title(course.title.toStdString());
//            res.set_art(course.art.toStdString());
//            res["subtitle"] = course.subTitle.toStdString();
//            res["description"] = course.description.toStdString();
//            if (!course.video.isEmpty())
//                res["video_url"] = course.video.toStdString();

//            // Push the result
//            if (!reply->push(res))
//                return;
//        }

//        // --------------------------------- Udacity

//        UdacityClient udacity(m_config);
//        QString udacityCatName = udacity.name();
//        auto udacityList = udacity.courses(QString::fromStdString(query_string));

//        // Register a category for the current weather, with the title we just built
//        auto udacityCategory = reply->register_category(udacityCatName.toLower().toStdString(),
//                                                         udacityCatName.toStdString(),
//                                                         "", sc::CategoryRenderer(CURRENT_TEMPLATE));

//        for (const auto &course : udacityList)
//        {
//            // Create a single result for the current weather category
//            sc::CategorisedResult res(udacityCategory);

//            // We must have a URI
//            res.set_uri(course.link.toStdString());
//            res.set_title(course.title.toStdString());
//            res.set_art(course.art.toStdString());
//            res["subtitle"] = course.subTitle.toStdString();
//            res["description"] = course.description.toStdString();
//            if (!course.video.isEmpty())
//                res["video_url"] = course.video.toStdString();

//            // Push the result
//            if (!reply->push(res))
//                return;
//        }

//        // --------------------------------- edX

//        EdxClient edx(m_config);
//        QString edxCatName = edx.name();
//        auto edxList = edx.courses(QString::fromStdString(query_string));

//        // Register a category for the current weather, with the title we just built
//        auto edxCategory = reply->register_category(edxCatName.toLower().toStdString(),
//                                                         edxCatName.toStdString(),
//                                                         "", sc::CategoryRenderer(CURRENT_TEMPLATE));

//        for (const auto &course : edxList)
//        {
//            // Create a single result for the current weather category
//            sc::CategorisedResult res(edxCategory);

//            // We must have a URI
//            res.set_uri(course.link.toStdString());
//            res.set_title(course.title.toStdString());
//            res.set_art(course.art.toStdString());
//            res["subtitle"] = course.subTitle.toStdString();
//            res["description"] = course.description.toStdString();
//            if (!course.video.isEmpty())
//                res["video_url"] = course.video.toStdString();

//            // Push the result
//            if (!reply->push(res))
//                return;
//        }

    } catch (domain_error &e) {
        // Handle exceptions being thrown by the client API
        cerr << e.what() << endl;
        reply->error(current_exception());
    }
}

