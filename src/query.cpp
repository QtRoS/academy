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

        // ----------------------- Coursera

        CourseraClient coursera(m_config);
        QString courseraCatName = coursera.name();
        auto courseraList = coursera.courses(QString::fromStdString(query_string));

        // Register a category for the current weather, with the title we just built
        auto courseraCategory = reply->register_category(courseraCatName.toLower().toStdString(),
                                                         courseraCatName.toStdString(),
                                                         "", sc::CategoryRenderer(CURRENT_TEMPLATE));

        for (const auto &course : courseraList)
        {
            // Create a single result for the current weather category
            sc::CategorisedResult res(courseraCategory);

            // We must have a URI
            res.set_uri(course.link.toStdString());
            res.set_title(course.title.toStdString());
            res.set_art(course.art.toStdString());
            res["subtitle"] = course.description.toStdString();
            res["description"] = course.description.toStdString();

            // Push the result
            if (!reply->push(res))
                return;
        }

        // --------------------------------- Udacity

        UdacityClient udacity(m_config);
        QString udacityCatName = udacity.name();
        auto udacityList = udacity.courses(QString::fromStdString(query_string));

        // Register a category for the current weather, with the title we just built
        auto udacityCategory = reply->register_category(udacityCatName.toLower().toStdString(),
                                                         udacityCatName.toStdString(),
                                                         "", sc::CategoryRenderer(CURRENT_TEMPLATE));

        for (const auto &course : udacityList)
        {
            // Create a single result for the current weather category
            sc::CategorisedResult res(udacityCategory);

            // We must have a URI
            res.set_uri(course.link.toStdString());
            res.set_title(course.title.toStdString());
            res.set_art(course.art.toStdString());
            res["subtitle"] = course.description.toStdString();
            res["description"] = course.description.toStdString();

            // Push the result
            if (!reply->push(res))
                return;
        }

        // --------------------------------- edX

        EdxClient edx(m_config);
        QString edxCatName = edx.name();
        auto edxList = edx.courses(QString::fromStdString(query_string));

        // Register a category for the current weather, with the title we just built
        auto edxCategory = reply->register_category(edxCatName.toLower().toStdString(),
                                                         edxCatName.toStdString(),
                                                         "", sc::CategoryRenderer(CURRENT_TEMPLATE));

        for (const auto &course : edxList)
        {
            // Create a single result for the current weather category
            sc::CategorisedResult res(edxCategory);

            // We must have a URI
            res.set_uri(course.link.toStdString());
            res.set_title(course.title.toStdString());
            res.set_art(course.art.toStdString());
            res["subtitle"] = course.description.toStdString();
            res["description"] = course.description.toStdString();

            // Push the result
            if (!reply->push(res))
                return;
        }

    } catch (domain_error &e) {
        // Handle exceptions being thrown by the client API
        cerr << e.what() << endl;
        reply->error(current_exception());
    }
}

