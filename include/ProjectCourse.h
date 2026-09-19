#ifndef PROJECTCOURSE_H
#define PROJECTCOURSE_H

#include "Course.h"
#include <string>
using namespace std;

class ProjectCourse : public Course {
private:
    bool isPassFail;

public:
    ProjectCourse(string code, string title, int creditVal, int capacity,
                  string lectId, bool isPassFail);
    ~ProjectCourse();

    bool getIsPassFail() const;

    int calculateCredits() const override;
    string toLine() const override;
};

#endif