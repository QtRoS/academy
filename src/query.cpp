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

Q_LOGGING_CATEGORY(Qry, "Query")

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
    sc::SearchQueryBase(query, metadata),
    //client_(config),
    m_config(config),
    m_coursera(config),
    m_udemy(config),
    m_edx(config),
    m_udacity(config)
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
}


void Query::run(sc::SearchReplyProxy const& reply)
{
    try {
        // Start by getting information about the query
        const sc::CannedQuery &query(sc::SearchQueryBase::query());

        // Get the query string
        string query_string = query.query_string();
        qCDebug(Qry) << "Query string is:" << QString::fromStdString(query_string);

        // Shared ptr way.
//        QList<QSharedPointer<BaseClient>> sources;
//        if (settings().at("coursera").get_bool())
//            sources.append(QSharedPointer<BaseClient>(new CourseraClient(m_config)));
//        if (settings().at("udemy").get_bool())
//            sources.append(QSharedPointer<BaseClient>(new UdemyClient(m_config)));
//        if (settings().at("edx").get_bool())
//            sources.append(QSharedPointer<BaseClient>(new EdxClient(m_config)));
//        if (settings().at("udacity").get_bool())
//            sources.append(QSharedPointer<BaseClient>(new UdacityClient(m_config)));

        QList<BaseClient*> sources;
        if (settings().at("coursera").get_bool())
            sources.append(&m_coursera);
        if (settings().at("udemy").get_bool())
            sources.append(&m_udemy);
        if (settings().at("edx").get_bool())
            sources.append(&m_edx);
        if (settings().at("udacity").get_bool())
            sources.append(&m_udacity);

        qCDebug(Qry) << "Source count:" << sources.count();

        QSet<QString> uniqueSet;

        for(int i = 0; i < sources.count(); i++)
        {
            QString sourceCatName = sources[i]->name();
            auto sourceList = sources[i]->courses(QString::fromStdString(query_string));

            auto sourceCategory = reply->register_category(sourceCatName.toLower().toStdString(),
                                                             sourceCatName.toStdString(),
                                                             "", sc::CategoryRenderer(CURRENT_TEMPLATE));

            //qCDebug(Qry) << "Processing source:" << sourceCatName;

            for (const auto &course : sourceList)
            {
                // Duplicate results cause Dash to crash.
                if (uniqueSet.contains(course.link))
                {
                    qCDebug(Qry) << "Duplicate:" << course.link;
                    continue;
                }
                uniqueSet.insert(course.link);


                sc::CategorisedResult res(sourceCategory);

                // We must have a URI
                res.set_uri(course.link.toStdString());
                res.set_title(course.title.toStdString());
                res.set_art(course.art.toStdString());
                res["headline"] = course.headline.toStdString();
                res["description"] = course.description.toStdString();
                res["source"] = sourceCatName.toStdString();
                res["extra"] = course.extra.toStdString();
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

            qCDebug(Qry) << "Finished with source:" << sourceCatName;
        }

    } catch (domain_error &e) {
        // Handle exceptions being thrown by the client API
        cerr << e.what() << endl;
        reply->error(current_exception());
    }
}

