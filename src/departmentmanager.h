#ifndef DEPARTMENTMANAGER_H
#define DEPARTMENTMANAGER_H

#include <QString>
#include <QList>
#include <QHash>

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
    static bool isMatch(const Course& course, const QString& department);
};

#endif // DEPARTMENTMANAGER_H
