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
#include "cachedclient.h"
#include "searchengine.h"
#include "localcache.h"

Q_DECLARE_LOGGING_CATEGORY(Edx)

class EdxClient : public CachedClient
{
public:

    EdxClient(Config::Ptr config);
    virtual ~EdxClient() = default;

    virtual QList<Course> courses(const QString& query) override;
    virtual const QString baseApiUrl() const override;
    virtual const QString name() const override;

private:
    QString grabExtra(const QDomElement& courseElem);
};

#endif // EDXCLIENT_H_

