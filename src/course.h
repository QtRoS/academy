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

#include <string>
#include <vector>
using namespace std;

struct Instructor
{
    string name;
    string bio;
    string image;
};

struct Course
{
    string id;
    string slug;
    string art;
    string title;
    string description;
    string headline;
    string link;
    string video;
    string extra;
    vector<Instructor> instructors;
    vector<string> departments;
};

#endif // COURSE_H_
