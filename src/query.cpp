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

        CourseraClient coursera(m_config);
        auto list = coursera.courses(QString::fromStdString(query_string));

        // Register a category for the current weather, with the title we just built
        auto courseraCategory = reply->register_category("coursera", "Coursera", "",
                                                     sc::CategoryRenderer(CURRENT_TEMPLATE));

        for (const auto &course : list)
        {
            // Create a single result for the current weather category
            sc::CategorisedResult res(courseraCategory);

            // We must have a URI
            res.set_uri(course.id.toStdString());
            res.set_title(course.name.toStdString());
            res.set_art(course.art.toStdString());
            res["subtitle"] = course.description.toStdString();
            res["description"] = course.description.toStdString();

            // Push the result
            if (!reply->push(res))
                return;
        }

        /// Populate current weather category

        // the Client is the helper class that provides the results
        // without mixing APIs and scopes code.
        // Add your code to retreive xml, json, or any other kind of result
        // in the client.
        Client::Current current;
        if (query_string.empty()) {
            // If the string is empty, get the current weather for London
            current = client_.weather("London,uk");
        } else {
            // otherwise, get the current weather for the search string
            current = client_.weather(query_string);
        }

        // Build up the description for the city
        stringstream ss(stringstream::in | stringstream::out);
        ss << current.city.name << ", " << current.city.country;

        // Register a category for the current weather, with the title we just built
        auto location_cat = reply->register_category("current", ss.str(), "",
                                                     sc::CategoryRenderer(CURRENT_TEMPLATE));

        {
            // Create a single result for the current weather category
            sc::CategorisedResult res(location_cat);

            // We must have a URI
            res.set_uri(to_string(current.city.id));

            // Build up the description for the current weather
            stringstream ss(stringstream::in | stringstream::out);
            ss << setprecision(3) << current.weather.temp.cur;
            ss << "°C";
            res.set_title(ss.str());

            // Set the rest of the attributes, art, description, etc
            res.set_art(current.weather.icon);
            res["subtitle"] = current.weather.description;
            res["description"] = "A description of the result";

            // Push the result
            if (!reply->push(res)) {
                // If we fail to push, it means the query has been cancelled.
                // So don't continue;
                return;
            }
        }

        /// Populate weather forecast category

        Client::Forecast forecast;
        if (query_string.empty()) {
            // If there is no search string, get the forecast for London
            forecast = client_.forecast_daily("London,uk");
        } else {
            // otherwise, get the forecast for the search string
            forecast = client_.forecast_daily(query_string);
        }

        // Register a category for the forecast
        auto forecast_cat = reply->register_category("forecast", _("7 day forecast"), "",
                                                     sc::CategoryRenderer(FORECAST_TEMPLATE));

        // For each of the forecast days
        for (const auto &weather : forecast.weather) {
            // Create a result
            sc::CategorisedResult res(forecast_cat);

            // We must have a URI
            res.set_uri(to_string(weather.id));

            // Build the description for the result
            stringstream ss(stringstream::in | stringstream::out);
            ss << setprecision(3) << weather.temp.max;
            ss << "°C to ";
            ss << setprecision(3) << weather.temp.min;
            ss << "°C";
            res.set_title(ss.str());

            // Set the rest of the attributes
            res.set_art(weather.icon);
            res["subtitle"] = weather.description;
            res["description"] = "A description of the result";

            // Push the result
            if (!reply->push(res)) {
                // If we fail to push, it means the query has been cancelled.
                // So don't continue;
                return;
            }
        }

    } catch (domain_error &e) {
        // Handle exceptions being thrown by the client API
        cerr << e.what() << endl;
        reply->error(current_exception());
    }
}

