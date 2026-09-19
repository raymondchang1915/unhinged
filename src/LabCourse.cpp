#include "LabCourse.h"

LabCourse::LabCourse(string c, string t, int cv, int cap, string lect, int lh)
    : Course(c, t, cv, cap, lect), labHours(lh) {
}

LabCourse::~LabCourse() {
}

int LabCourse::getLabHours() const {
    return labHours;
}

// lab work adds credit: 3 contact hours = 1 credit
int LabCourse::calculateCredits() const {
    return getCreditVal() + labHours / 3;
}

string LabCourse::toLine() const {
    return "LAB," + Course::toLine() + "," + to_string(labHours) + "," + prereqToLine() + "," + slotsToLine();
}