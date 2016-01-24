#ifndef EDXCLIENT_H_
#define EDXCLIENT_H_

#include <atomic>
#include <deque>
#include <map>
#include <memory>
#include <string>
#include <core/net/http/request.h>
#include <core/net/uri.h>

#include <QtXml/QDomDocument>
#include <QtXml/QDomNode>
#include <QtXml/QDomElement>
#include <QtXml/QDomNodeList>
#include <QString>
#include <QList>
#include <QDebug>

#include "config.h"
#include "baseclient.h"
#include "searchengine.h"

Q_DECLARE_LOGGING_CATEGORY(Edx)

class EdxClient : public BaseClient
{
public:

    EdxClient(Config::Ptr config);
    virtual ~EdxClient() = default;

    virtual QList<Course> courses(const QString& query) override;
    virtual const QString baseApiUrl() const override;
    virtual const QString name() const override;
};

#endif // EDXCLIENT_H_

