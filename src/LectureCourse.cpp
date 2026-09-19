#include "LectureCourse.h"

LectureCourse::LectureCourse(string c, string t, int cv, int cap, string lect)
    : Course(c, t, cv, cap, lect) {
}

LectureCourse::~LectureCourse() {
}

// plain lecture course: credits are taken as declared
int LectureCourse::calculateCredits() const {
    return getCreditVal();
}

string LectureCourse::toLine() const {
    return "LEC|" + Course::toLine();
}