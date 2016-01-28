#ifndef COURSE_H_
#define COURSE_H_

class QString;

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
    // Unstable fields below.
    QString university;
    QString additionalInfo;
};

struct Instructor
{
    QString name;
    QString bio;
    QString image;
};

#endif // COURSE_H_
