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
    QString subTitle; // ?
    QString description;
    QString shortDescription;  // ?
    QString link;
    QString video;
    QList<Instructor> instructors;
    // Unstable fields below.
    QString university;
    QString additionalInfo;
};

#endif // COURSE_H_
