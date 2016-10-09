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

#ifndef SEARCHENGINE_H_
#define SEARCHENGINE_H_

#include <QRegExp>

#include "course.h"

class SearchEngine
{
public:
    explicit SearchEngine(const QString& query)
    {
        QStringList parts = query.split(' ');
        for (int i = 0; i < parts.size(); ++i)
            parts[i] = "(" + parts[i].toLower().replace("+", "\\+") + ")";
        QString regex = parts.join('|');
        m_regex = QRegExp(regex, Qt::CaseInsensitive);
    }

    bool isMatch(const Course& course)
    {
        // TODO
        return m_regex.indexIn(QString::fromStdString(course.title)) != -1 ||
                m_regex.indexIn(QString::fromStdString(course.headline)) != -1 ||
                m_regex.indexIn(QString::fromStdString(course.description)) != -1;
    }

private:
    QRegExp m_regex;
};

#endif // SEARCHENGINE_H_
