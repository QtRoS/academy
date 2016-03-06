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

#ifndef COURSE_H_
#define COURSE_H_

#include <QList>
#include <QStringList>

class QString;

struct Instructor
{
    QString name;
    QString bio;
    QString image;
};

struct Course
{
    QString id;
    QString slug;
    QString art;
    QString title;
    QString description;
    QString headline;
    QString link;
    QString video;
    QString extra;
    QList<Instructor> instructors;
    QStringList departments;
};

#endif // COURSE_H_
