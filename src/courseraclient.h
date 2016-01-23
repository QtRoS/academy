#ifndef COURSERACLIENT_H_
#define COURSERACLIENT_H_

#include <atomic>
#include <deque>
#include <map>
#include <memory>
#include <string>
#include <core/net/http/request.h>
#include <core/net/uri.h>

#include <QJsonDocument>
#include <QString>
#include <QList>

#include "config.h"

/**
 * Provide a nice way to access the HTTP API.
 *
 * We don't want our scope's code to be mixed together with HTTP and JSON handling.
 */
class CourseraClient
{
public:

    struct Course
    {
        QString id;
        QString art;
        QString name;
        QString description;
        QString university;
        QString lector; // TODO
    };

    CourseraClient(Config::Ptr config);

    virtual ~CourseraClient() = default;

    virtual QList<Course> courses(const QString& query);

    /**
     * Cancel any pending queries (this method can be called from a different thread)
     */
    virtual void cancel();

    virtual Config::Ptr config();

protected:
    void get(const core::net::Uri::Path &path, const core::net::Uri::QueryParameters &parameters, QJsonDocument &root);
    /**
     * Progress callback that allows the query to cancel pending HTTP requests.
     */
    core::net::http::Request::Progress::Next progress_report(const core::net::http::Request::Progress& progress);

    /**
     * Hang onto the configuration information
     */
    Config::Ptr m_config;

    /**
     * Thread-safe cancelled flag
     */
    std::atomic<bool> m_cancelled;
};

#endif // COURSERACLIENT_H_

