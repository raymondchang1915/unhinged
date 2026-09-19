#include "Enrolment.h"

// static members must be defined once, outside the class
int Enrolment::nextId = 1;

// new enrolment: the ID is generated
Enrolment::Enrolment(string sid, string cid, string date)
    : studentId(sid), courseId(cid), enrolmentDate(date) {
    id = "E" + to_string(nextId);
    nextId++;
}

// loaded enrolment: the ID comes from the file
Enrolment::Enrolment(string i, string sid, string cid, string date)
    : id(i), studentId(sid), courseId(cid), enrolmentDate(date) {
}

string Enrolment::getId() const {
    return id;
}

string Enrolment::getStudentId() const {
    return studentId;
}

string Enrolment::getCourseId() const {
    return courseId;
}

string Enrolment::getEnrolmentDate() const {
    return enrolmentDate;
}

string Enrolment::toLine() const {
    return id + "|" + studentId + "|" + courseId + "|" + enrolmentDate;
}

// sorts by enrolment ID
bool Enrolment::operator<(const Enrolment& other) const {
    return id < other.id;
}

ostream& operator<<(ostream& os, const Enrolment& e) {
    os << e.getId() << ": student " << e.getStudentId()
       << " in course " << e.getCourseId()
       << " on " << e.getEnrolmentDate();
    return os;
}