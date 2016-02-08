#ifndef COURSE_H_
#define COURSE_H_

#include <QList>

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
};

#endif // COURSE_H_
