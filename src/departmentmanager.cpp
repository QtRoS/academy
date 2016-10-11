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

#include "departmentmanager.h"
#include <localization.h>

#include <algorithm>

DepartmentManager::DepartmentManager()
{ }

vector<Department> DepartmentManager::departments()
{
    static vector<Department> list;

    if (list.empty())
    {
        list.push_back(Department("art",       _("Art and Design")));
        list.push_back(Department("business",  _("Business")));
        list.push_back(Department("music",     _("Music")));
        list.push_back(Department("languages", _("Languages")));
        list.push_back(Department("it",        _("IT and Tech")));
        list.push_back(Department("science",   _("Science")));
        list.push_back(Department("humanities",_("Humanities")));
        list.push_back(Department("personal",  _("Personal")));
        list.push_back(Department("other",     _("Other")));
    }

    return list;
}

multimap<string, string> DepartmentManager::mapping()
{
    static multimap<string, string> hash;

    if (!hash.size())
    {
        // Coursera.
        hash.insert(std::pair<string,string>("arts-and-humanities", "art"));
        hash.insert(std::pair<string,string>("arts-and-humanities", "humanities"));
        hash.insert(std::pair<string,string>("business", "business"));
        hash.insert(std::pair<string,string>("computer-science", "it"));
        hash.insert(std::pair<string,string>("data-science", "it"));
        hash.insert(std::pair<string,string>("language-learning", "languages"));
        hash.insert(std::pair<string,string>("life-sciences", "science"));
        hash.insert(std::pair<string,string>("math-and-logic", "science"));
        hash.insert(std::pair<string,string>("personal-development", "personal"));
        hash.insert(std::pair<string,string>("physical-science-and-engineering", "science"));
        hash.insert(std::pair<string,string>("social-sciences", "science"));
        // Udacity.
        hash.insert(std::pair<string,string>("Data Science", "it"));
        hash.insert(std::pair<string,string>("Web Development", "it"));
        hash.insert(std::pair<string,string>("Software Engineering", "it"));
        hash.insert(std::pair<string,string>("Android", "it"));
        hash.insert(std::pair<string,string>("iOS", "it"));
        hash.insert(std::pair<string,string>("Georgia Tech Masters in CS", "it"));
        hash.insert(std::pair<string,string>("Non-Tech", "other"));
        // Edx
        hash.insert(std::pair<string,string>("Art & Culture", "art"));
        hash.insert(std::pair<string,string>("Biology & Life Sciences", "science"));
        hash.insert(std::pair<string,string>("Business & Management", "business"));
        hash.insert(std::pair<string,string>("Communication", "other"));
        hash.insert(std::pair<string,string>("Computer Science", "it"));
        hash.insert(std::pair<string,string>("Data Analysis & Statistics", "it"));
        hash.insert(std::pair<string,string>("Economics & Finance", "business"));
        hash.insert(std::pair<string,string>("Education & Teacher Training", "personal"));
        hash.insert(std::pair<string,string>("Electronics", "it"));
        hash.insert(std::pair<string,string>("Energy & Earth Sciences", "science"));
        hash.insert(std::pair<string,string>("Engineering", "it"));
        hash.insert(std::pair<string,string>("Environmental Studies", "science"));
        hash.insert(std::pair<string,string>("Health & Safety", "other"));
        hash.insert(std::pair<string,string>("History", "humanities"));
        hash.insert(std::pair<string,string>("Humanities", "humanities"));
        hash.insert(std::pair<string,string>("Language", "languages"));
        hash.insert(std::pair<string,string>("Law", "humanities"));
        hash.insert(std::pair<string,string>("Literature", "humanities"));
        hash.insert(std::pair<string,string>("Math", "science"));
        hash.insert(std::pair<string,string>("Medicine", "other"));
        hash.insert(std::pair<string,string>("Philosophy & Ethics", "humanities"));
        hash.insert(std::pair<string,string>("Physics", "science"));
        hash.insert(std::pair<string,string>("Science", "science"));
        hash.insert(std::pair<string,string>("Social Sciences", "humanities"));
        // Udemy
        hash.insert(std::pair<string,string>("Academics", "other"));
        hash.insert(std::pair<string,string>("Business", "business"));
        hash.insert(std::pair<string,string>("Crafts & Hobbies", "other"));
        hash.insert(std::pair<string,string>("Design", "art"));
        hash.insert(std::pair<string,string>("Development", "it"));
        hash.insert(std::pair<string,string>("Games", "other"));
        hash.insert(std::pair<string,string>("Health & Fitness", "other"));
        hash.insert(std::pair<string,string>("Humanities", "humanities"));
        hash.insert(std::pair<string,string>("IT & Software", "it"));
        hash.insert(std::pair<string,string>("Language", "languages"));
        hash.insert(std::pair<string,string>("Lifestyle", "other"));
        hash.insert(std::pair<string,string>("Marketing", "business"));
        hash.insert(std::pair<string,string>("Math & Science", "science"));
        hash.insert(std::pair<string,string>("Music", "music"));
        hash.insert(std::pair<string,string>("Office-Productivity", "business"));
        hash.insert(std::pair<string,string>("Other", "other"));
        hash.insert(std::pair<string,string>("Personal-Development", "personal"));
        hash.insert(std::pair<string,string>("Photography", "art"));
        hash.insert(std::pair<string,string>("Social-Science", "humanities"));
        hash.insert(std::pair<string,string>("Sports", "other"));
        hash.insert(std::pair<string,string>("Teacher-Training", "personal"));
        hash.insert(std::pair<string,string>("Technology", "it"));
        hash.insert(std::pair<string,string>("Test", "it"));
        hash.insert(std::pair<string,string>("Test-Prep", "it"));
        // iversity
        hash.insert(std::pair<string,string>("Agricultural studies", "other"));
        hash.insert(std::pair<string,string>("Art studies", "art"));
        hash.insert(std::pair<string,string>("Economics", "business"));
        hash.insert(std::pair<string,string>("Education", "personal"));
        hash.insert(std::pair<string,string>("Engineering", "it"));
        hash.insert(std::pair<string,string>("Environmental Studies", "other"));
        hash.insert(std::pair<string,string>("Health", "personal"));
        hash.insert(std::pair<string,string>("History", "humanities"));
        hash.insert(std::pair<string,string>("Interdisciplinary", "other"));
        hash.insert(std::pair<string,string>("Languages", "languages"));
        hash.insert(std::pair<string,string>("Literature", "humanities"));
        hash.insert(std::pair<string,string>("Medicine", "other"));
        hash.insert(std::pair<string,string>("Philosophy", "humanities"));
        hash.insert(std::pair<string,string>("Social Sciences & Humanities ", "humanities"));
        // openlearning
        hash.insert(std::pair<string,string>("Arts and Design", "art"));
        hash.insert(std::pair<string,string>("Business and Economics", "business"));
        hash.insert(std::pair<string,string>("Computers and Technology", "it"));
        hash.insert(std::pair<string,string>("Education", "personal"));
        hash.insert(std::pair<string,string>("Engineering", "it"));
        hash.insert(std::pair<string,string>("Health and Medicine", "other"));
        hash.insert(std::pair<string,string>("Humanities", "humanities"));
        hash.insert(std::pair<string,string>("Language", "languages"));
        hash.insert(std::pair<string,string>("Language and Communication", "languages"));
        hash.insert(std::pair<string,string>("Law", "humanities"));
        hash.insert(std::pair<string,string>("Lifestyle", "personal"));
        hash.insert(std::pair<string,string>("Math and Science", "science"));
        hash.insert(std::pair<string,string>("Other", "other"));
        hash.insert(std::pair<string,string>("Self Improvement", "personal"));
        hash.insert(std::pair<string,string>("Sports and Fitness", "personal"));
        hash.insert(std::pair<string,string>("Technology", "it"));
    }

    return hash;
}

bool DepartmentManager::isMatch(const Course &course, const string &department)
{
    const auto& hash = mapping();

    for(uint i = 0; i < course.departments.size(); ++i)
    {
        auto its = hash.equal_range(course.departments[i]);
        for (auto iter = its.first; iter != its.second; ++iter)
        {
            if (iter->second == department)
                return true;
        }
    }

    return false;
}
