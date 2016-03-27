/*
 * Copyright (C) 2016 Roman Shchekin aka mrqtros.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <templateclient.h>
#include <localization.h>

#include <core/net/error.h>
#include <core/net/http/client.h>
#include <core/net/http/content_type.h>
#include <core/net/http/response.h>
#include <QVariantMap>

Q_LOGGING_CATEGORY(Template, "Template")

namespace http = core::net::http;
namespace net = core::net;

using namespace std;

TemplateClient::TemplateClient(Config::Ptr config) :
    CachedClient(config)
{ }

QList<Course> TemplateClient::courses(const QString &query)
{
    QList<Course> list;

    QByteArray data;
    net::Uri::Path path;
    net::Uri::QueryParameters params;

    params.push_back({"page_size", "80"});

    if (!query.isEmpty())
    {
        params.push_back({"search", query.toStdString()});
        setCacheEnabled(false);
    }

    qCDebug(Template) << "Download started...";
    get( path, params, data);
    qCDebug(Template) << "Data received:" << data.length() << "bytes";
    QJsonDocument root = QJsonDocument::fromJson(data);

    QVariantMap variant = root.toVariant().toMap();
    QList<QVariant> courses = variant["results"].toList();
    qCDebug(Template) << "Element count:" << courses.length();

    for (const QVariant &i : courses)
    {
        QVariantMap map = i.toMap();

        Course course;
        course.id = map["id"].toString();
        course.slug = map["url"].toString();
        course.title = map["title"].toString();
        course.description = map["description"].toString();
        course.headline = map["headline"].toString();
        course.art = map["image_480x270"].toString();
        course.link = QStringLiteral("https://www.Template.com") + map["url"].toString();
        course.extra = grabExtra(map);
        //course.video = map["teaser_video"].toMap()["youtube_url"].toString();

        QList<QVariant> instructors = map["visible_instructors"].toList();
        for (const QVariant& j : instructors)
        {
            QVariantMap imap = j.toMap();
            Instructor instr;
            instr.image = imap["image_100x100"].toString();
            instr.bio = imap["job_title"].toString();
            instr.name = imap["title"].toString();

            course.instructors.append(instr);
        }

        QVariantMap kmap = map["primary_category"].toMap();
        course.departments.append(kmap["title"].toString());

        //qCDebug(Template) << "Category count: " << course.departments;
        //qCDebug(Template) << "Instr count: " << course.instructors.size();
        list.append(course);
    }

    return list;
}

const QString TemplateClient::baseApiUrl() const
{
    return QStringLiteral("https://www.template.com/api-2.0/courses");
}

const QString TemplateClient::name() const
{
    return QStringLiteral("Template");
}

const QMap<QByteArray, QByteArray> TemplateClient::customHeaders() const
{
    QMap<QByteArray, QByteArray> res;
    res.insert("Authorization", "Basic MlloUmZ1TXpUSjJLMjJmZWZoSldTeVoyanVtOWx0dkdoWFhFUWZQaTpiNGRIUXhmUDdsODVWa3RHQlM4dUFpdU5ZclpyOEZWY3E3cFpTaWRXbVNMSTBuNm5mWGFyRUxSQ2xqdEtDbjZPcTR3ZkZwWjlqM0RsdU13aUhDN0UxVW1zS1YyQzRtSUlvR2ZEYXpNYVhtbDZjRGtHcjJmOHVqVzVkQ2J5VThaaw==");
    return res;
}

QString TemplateClient::grabExtra(const QVariantMap &map)
{
    return _("price - ") + map["price"].toString();
    //    QStringList extra;
    //    extra << QStringLiteral("price - ") + map["price"].toString();
    //    return extra.join(", ");
}
