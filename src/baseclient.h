#ifndef BASECLIENT_H_
#define BASECLIENT_H_

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
#include <QDebug>

#include "config.h"

/**
 * Provide a nice way to access the HTTP API.
 *
 * We don't want our scope's code to be mixed together with HTTP and JSON handling.
 */
class BaseClient
{
public:

    struct Course
    {
        QString id;
        QString slug;
        QString art;
        QString title;
        QString subTitle;
        QString description;
        QString shortDescription;
        QString university;
        QString lector; // TODO
        QString source;
        QString additionalInfo;
    };

    BaseClient(Config::Ptr config);

    virtual ~BaseClient() = default;

    virtual QList<Course> courses(const QString& query) = 0;
    virtual const QString baseApiUrl() const = 0;
    virtual const QString name() const = 0;

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

#endif // BASECLIENT_H_

