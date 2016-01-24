#include <edxclient.h>

#include <core/net/error.h>
#include <core/net/http/client.h>
#include <core/net/http/content_type.h>
#include <core/net/http/response.h>
#include <QVariantMap>

namespace http = core::net::http;
namespace net = core::net;

using namespace std;

EdxClient::EdxClient(Config::Ptr config) :
    BaseClient(config)
{

}

QList<EdxClient::Course> EdxClient::courses(const QString &query)
{
    QList<EdxClient::Course> list;


    QByteArray data;
    net::Uri::Path path;
    net::Uri::QueryParameters params;

    qDebug() << "Going to download edx data";
    get( path, params, data);
    qDebug() << "Download finished" << data.length();

    // qDebug() << "EDX" << data;

    QDomDocument doc;

    if (!doc.setContent(data))
    {
        // TODO LOG ERROR
        return list;
    }

    QDomNodeList itemList = doc.elementsByTagName("item");
    qDebug() << "ITEM COUNT" << itemList.size();

    for (int i = 0; i < itemList.length(); i++)
    {
        QDomElement courseElem = itemList.at(i).toElement();

        if (courseElem.isNull())
            continue;

        Course course;

        QDomElement id = courseElem.firstChildElement("guid");
        if (!id.isNull())
            course.id = id.text();

        QDomElement title = courseElem.firstChildElement("title");
        if (!title.isNull())
            course.title = title.text();

        QDomElement subTitle = courseElem.firstChildElement("course:subtitle");
        if (!subTitle.isNull())
            course.subTitle = subTitle.text();

        QDomElement description = courseElem.firstChildElement("description");
        if (!description.isNull())
            course.description = description.text();

        course.shortDescription = course.description.left(100) + QStringLiteral("...");

        QDomElement art = courseElem.firstChildElement("course:image-thumbnail");
        if (!art.isNull())
            course.art = art.text();

        list.append(course);
    }

    return list;
}

const QString EdxClient::baseApiUrl() const
{
    return QStringLiteral("https://www.edx.org/api/v2/report/course-feed/rss");
}

const QString EdxClient::name() const
{
    return QStringLiteral("edX");
}