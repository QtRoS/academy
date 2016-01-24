#ifndef COURSE_H_
#define COURSE_H_

class QString;

struct Course
{
    QString id;
    QString slug;
    QString art;
    QString title;
    QString subTitle;
    QString description;
    QString shortDescription;
    // Unstable fields below.
    QString university;
    QString lector;
    QString additionalInfo;
};

#endif // COURSE_H_
