#include "departmentmanager.h"

DepartmentManager::DepartmentManager()
{

}

QList<Department> DepartmentManager::departments()
{
    QList<Department> list;

    // list << Department("", "All");
    list << Department("art", "Art & Design");
    list << Department("business", "Business");
    list << Department("music", "Music");
    list << Department("languages", "Languages");
    list << Department("it", "IT & Tech");
    list << Department("science", "Science");
    list << Department("humanities", "Humanities");
    list << Department("personal", "Personal");
    list << Department("other", "Other");

    return list;
}

bool DepartmentManager::isMatch(const Course &course, const QString &department)
{
    static QHash<QString, QString> hash;

    if (!hash.count())
    {
        // Coursera.
        hash.insert("arts-and-humanities", "art");
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
        hash.insert("Crafts-and-Hobbies", "other");
        hash.insert("Design", "art");
        hash.insert("Development", "it");
        hash.insert("Games", "other");
        hash.insert("Health-and-Fitness", "other");
        hash.insert("Humanities", "humanities");
        hash.insert("IT-and-Software", "it");
        hash.insert("Language", "languages");
        hash.insert("Lifestyle", "other");
        hash.insert("Marketing", "business");
        hash.insert("Math-and-Science", "science");
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
    }

    for(int i = 0; i < course.departments.length(); i++)
        if (hash[course.departments[i]] == department)
            return true;

    return false;
}

