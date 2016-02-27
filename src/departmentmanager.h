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
