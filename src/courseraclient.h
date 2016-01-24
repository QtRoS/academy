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
#include <QDebug>

#include "config.h"
#include "baseclient.h"

/**
 * Provide a nice way to access the HTTP API.
 *
 * We don't want our scope's code to be mixed together with HTTP and JSON handling.
 */
class CourseraClient : public BaseClient
{
public:
    CourseraClient(Config::Ptr config);
    virtual ~CourseraClient() = default;

    virtual QList<Course> courses(const QString& query) override;
    virtual const QString baseApiUrl() const override;
    virtual const QString name() const override;
};

#endif // COURSERACLIENT_H_

