#ifndef LECTURE_COURSE_H
#define LECTURE_COURSE_H

#include "Course.h"
#include <string>
using namespace std;

class LectureCourse : public Course {
public:
    LectureCourse(string code, string title, int creditVal, int capacity, string lectId);
    ~LectureCourse();

    int calculateCredits() const override;
    string toLine() const override;
};

#endif
