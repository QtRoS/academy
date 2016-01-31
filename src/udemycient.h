#ifndef UDEMYCLIENT_H_
#define UDEMYCLIENT_H_

#include <atomic>
#include <deque>
#include <map>
#include <memory>
#include <string>
#include <core/net/http/request.h>
#include <core/net/uri.h>

#include <QJsonDocument>
#include <QJsonObject>
#include <QString>
#include <QList>
#include <QMap>
#include <QByteArray>
#include <QDebug>


#include "config.h"
#include "baseclient.h"
#include "searchengine.h"

Q_DECLARE_LOGGING_CATEGORY(Udemy)

class UdemyClient : public BaseClient
{
public:

    UdemyClient(Config::Ptr config);
    virtual ~UdemyClient() = default;

    virtual QList<Course> courses(const QString& query) override;
    virtual const QString baseApiUrl() const override;
    virtual const QString name() const override;
    virtual QMap<QByteArray, QByteArray> customHeaders() const override;

};

#endif // UDEMYCLIENT_H_

