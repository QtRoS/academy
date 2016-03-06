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

#ifndef DEPARTMENTMANAGER_H
#define DEPARTMENTMANAGER_H

#include <QString>
#include <QList>
#include <QHash>
#include <QStringList>

#include "course.h"

struct Department
{
    QString id;
    QString label;
    Department(const QString& i, const QString& l)
    {
        id = i;
        label = l;
    }
};

class DepartmentManager
{
public:
    DepartmentManager();
    static QList<Department> departments();
    static QHash<QString, QString> mapping();
    static bool isMatch(const Course& course, const QString& department);
    static QString flatDescription(const QString& deps);
};

#endif // DEPARTMENTMANAGER_H
