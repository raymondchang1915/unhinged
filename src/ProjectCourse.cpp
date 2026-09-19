#include "ProjectCourse.h"

ProjectCourse::ProjectCourse(string c, string t, int cv, int cap, string lect, bool pf)
    : Course(c, t, cv, cap, lect), isPassFail(pf) {
}

ProjectCourse::~ProjectCourse() {
}

bool ProjectCourse::getIsPassFail() const {
    return isPassFail;
}

// pass/fail projects carry no GPA credit
int ProjectCourse::calculateCredits() const {
    if (isPassFail) {
        return 0;
    }
    return getCreditVal();
}

string ProjectCourse::toLine() const {
    return "PROJ|" + Course::toLine() + "|" + (isPassFail ? "1" : "0");
}