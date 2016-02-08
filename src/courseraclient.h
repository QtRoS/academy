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
#include <QMap>

#include "config.h"
#include "baseclient.h"
#include "cachedclient.h"

Q_DECLARE_LOGGING_CATEGORY(Coursera)

class CourseraClient : public CachedClient
{
public:
    CourseraClient(Config::Ptr config);
    virtual ~CourseraClient() = default;

    virtual QList<Course> courses(const QString& query) override;
    virtual const QString baseApiUrl() const override;
    virtual const QString name() const override;

private:
    QString grabExtra(const QVariantMap& map);
};

#endif // COURSERACLIENT_H_

