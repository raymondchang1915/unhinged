#ifndef LAB_COURSE_H
#define LAB_COURSE_H


#include "Course.h"
#include <string>
using namespace std;

class LabCourse : public Course {
private:
    int labHours;

public:
    LabCourse(string code, string title, int creditVal, int capacity,
              string lectId, int labHours);
    ~LabCourse();

    int getLabHours() const;

    int calculateCredits() const override;
    string toLine() const override;
};

#endif
