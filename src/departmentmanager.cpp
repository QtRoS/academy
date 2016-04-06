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

DepartmentManager::DepartmentManager()
{

}

QList<Department> DepartmentManager::departments()
{
    static QList<Department> list;

    if (list.empty())
    {
        list << Department("art",       _("Art and Design"));
        list << Department("business",  _("Business"));
        list << Department("music",     _("Music"));
        list << Department("languages", _("Languages"));
        list << Department("it",        _("IT and Tech"));
        list << Department("science",   _("Science"));
        list << Department("humanities",_("Humanities"));
        list << Department("personal",  _("Personal"));
        list << Department("other",     _("Other"));
    }

    return list;
}

QHash<QString, QString> DepartmentManager::mapping()
{
    static QHash<QString, QString> hash;

    if (!hash.count())
    {
        // Coursera.
        hash.insertMulti("arts-and-humanities", "art");
        hash.insertMulti("arts-and-humanities", "humanities");
        hash.insert("business", "business");
        hash.insert("computer-science", "it");
        hash.insert("data-science", "it");
        hash.insert("language-learning", "languages");
        hash.insert("life-sciences", "science");
        hash.insert("math-and-logic", "science");
        hash.insert("personal-development", "personal");
        hash.insert("physical-science-and-engineering", "science");
        hash.insert("social-sciences", "science");
        // Udacity.
        hash.insert("Data Science", "it");
        hash.insert("Web Development", "it");
        hash.insert("Software Engineering", "it");
        hash.insert("Android", "it");
        hash.insert("iOS", "it");
        hash.insert("Georgia Tech Masters in CS", "it");
        hash.insert("Non-Tech", "other");
        // Edx
        hash.insert("Art & Culture", "art");
        hash.insert("Biology & Life Sciences", "science");
        hash.insert("Business & Management", "business");
        hash.insert("Communication", "other");
        hash.insert("Computer Science", "it");
        hash.insert("Data Analysis & Statistics", "it");
        hash.insert("Economics & Finance", "business");
        hash.insert("Education & Teacher Training", "personal");
        hash.insert("Electronics", "it");
        hash.insert("Energy & Earth Sciences", "science");
        hash.insert("Engineering", "it");
        hash.insert("Environmental Studies", "science");
        hash.insert("Health & Safety", "other");
        hash.insert("History", "humanities");
        hash.insert("Humanities", "humanities");
        hash.insert("Language", "languages");
        hash.insert("Law", "humanities");
        hash.insert("Literature", "humanities");
        hash.insert("Math", "science");
        hash.insert("Medicine", "other");
        hash.insert("Philosophy & Ethics", "humanities");
        hash.insert("Physics", "science");
        hash.insert("Science", "science");
        hash.insert("Social Sciences", "humanities");
        // Udemy
        hash.insert("Academics", "other");
        hash.insert("Business", "business");
        hash.insert("Crafts & Hobbies", "other");
        hash.insert("Design", "art");
        hash.insert("Development", "it");
        hash.insert("Games", "other");
        hash.insert("Health & Fitness", "other");
        hash.insert("Humanities", "humanities");
        hash.insert("IT & Software", "it");
        hash.insert("Language", "languages");
        hash.insert("Lifestyle", "other");
        hash.insert("Marketing", "business");
        hash.insert("Math & Science", "science");
        hash.insert("Music", "music");
        hash.insert("Office-Productivity", "business");
        hash.insert("Other", "other");
        hash.insert("Personal-Development", "personal");
        hash.insert("Photography", "art");
        hash.insert("Social-Science", "humanities");
        hash.insert("Sports", "other");
        hash.insert("Teacher-Training", "personal");
        hash.insert("Technology", "it");
        hash.insert("Test", "it");
        hash.insert("Test-Prep", "it");
        // iversity
        hash.insert("Agricultural studies", "other");
        hash.insert("Art studies", "art");
        hash.insert("Economics", "business");
        hash.insert("Education", "personal");
        hash.insert("Engineering", "it");
        hash.insert("Environmental Studies", "other");
        hash.insert("Health", "personal");
        hash.insert("History", "humanities");
        hash.insert("Interdisciplinary", "other");
        hash.insert("Languages", "languages");
        hash.insert("Literature", "humanities");
        hash.insert("Medicine", "other");
        hash.insert("Philosophy", "humanities");
        hash.insert("Social Sciences & Humanities ", "humanities");
        // openlearning
        hash.insert("Arts and Design", "art");
        hash.insert("Business and Economics", "business");
        hash.insert("Computers and Technology", "it");
        hash.insert("Education", "personal");
        hash.insert("Engineering", "it");
        hash.insert("Health and Medicine", "other");
        hash.insert("Humanities", "humanities");
        hash.insert("Language", "languages");
        hash.insert("Language and Communication", "languages");
        hash.insert("Law", "humanities");
        hash.insert("Lifestyle", "personal");
        hash.insert("Math and Science", "science");
        hash.insert("Other", "other");
        hash.insert("Self Improvement", "personal");
        hash.insert("Sports and Fitness", "personal");
        hash.insert("Technology", "it");
    }

    return hash;
}

bool DepartmentManager::isMatch(const Course &course, const QString &department)
{
    const QHash<QString, QString> hash = mapping();

    for(int i = 0; i < course.departments.length(); i++)
    {
        // Some departments are mapped to multiple categories.
        if (department == "art" || department == "humanities")
        {
            if (hash.values(course.departments[i]).contains(department))
                return true;
        }
        else if (hash[course.departments[i]] == department)
            return true;
    }

    return false;
}

QString DepartmentManager::flatDescription(const QString& deps)
{
    const QList<Department> list = departments();
    const QHash<QString, QString> hash = mapping();

    QStringList names;
    QStringList courseDeps = deps.split(';');
    for(int i = 0; i < courseDeps.length(); i++)
    {
        QString dep = courseDeps[i];
        QList<QString> intCat = hash.values(dep);

        for (int j = 0; j < list.size(); ++j)
            for (int k = 0; k < intCat.size(); ++k)
                if (list[j].id == intCat[k] && !names.contains(list[j].label))
                    names << list[j].label;
    }

    return names.join(", ");
}

