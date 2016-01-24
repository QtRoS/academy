#ifndef UDACITYCLIENT_H_
#define UDACITYCLIENT_H_

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
#include "searchengine.h"

Q_DECLARE_LOGGING_CATEGORY(Udacity)

class UdacityClient : public BaseClient
{
public:

    UdacityClient(Config::Ptr config);
    virtual ~UdacityClient() = default;

    virtual QList<Course> courses(const QString& query) override;
    virtual const QString baseApiUrl() const override;
    virtual const QString name() const override;

};

#endif // UDACITYCLIENT_H_

