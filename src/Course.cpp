#include "Course.h"

Course::Course(string c, string t, int cv, int cap, string lect)
    : code(c), title(t), creditVal(cv), capacity(cap), assignedLectId(lect) {
}

Course::~Course() {
}

string Course::getId() const {
    return code;
}

string Course::getTitle() const {
    return title;
}

int Course::getCreditVal() const {
    return creditVal;
}

int Course::getCapacity() const {
    return capacity;
}

string Course::getAssignedLectId() const {
    return assignedLectId;
}

vector<string> Course::getPrereq() const {
    return prereq;
}

vector<string> Course::getEnrolledIds() const {
    return enrolledIds;
}

bool Course::isFull() const {
    return (int)enrolledIds.size() >= capacity;
}

void Course::addStudent(string studentId) {
    enrolledIds.push_back(studentId);
}

void Course::removeStudent(string studentId) {
    for (int i = 0; i < (int)enrolledIds.size(); i++) {
        if (enrolledIds[i] == studentId) {
            enrolledIds.erase(enrolledIds.begin() + i);
            return;
        }
    }
}

void Course::addPrereq(string courseCode) {
    prereq.push_back(courseCode);
}

string Course::toLine() const {
    return code + "|" + title + "|" + to_string(creditVal) + "|"
         + to_string(capacity) + "|" + assignedLectId;
}

ostream& operator<<(ostream& os, const Course& c) {
    os << c.getId() << " - " << c.getTitle()
       << " (" << c.calculateCredits() << " credits)";
    return os;
}
