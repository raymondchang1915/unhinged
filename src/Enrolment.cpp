#include "Enrolment.h"
#include "Exceptions.h"

#include <sstream>

int Enrolment::nextId = 1;

// new enrolment: the ID is generated
Enrolment::Enrolment(string sid, string cid, string date): studentId(sid), courseId(cid), enrolmentDate(date) 
{ id = "ENR" + to_string(nextId); 
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
    return id + "," + studentId + "," + courseId + "," + enrolmentDate;
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
// enrolments.txt format:  id,studentId,courseId,date
Enrolment* Enrolment::fromLine(string line) {
    stringstream ss(line);
    string id, studentId, courseId, date;

    getline(ss, id, ',');
    getline(ss, studentId, ',');
    getline(ss, courseId, ',');
    getline(ss, date);

    if (id == "" || studentId == "" || courseId == "") {
        throw CorruptDataException("enrolments.txt", 0);
    }

    // keep the counter ahead of every ID already on disk, so new enrolments never collide
    if (id.rfind("ENR", 0) == 0) {
        string digits = id.substr(3);
        bool allDigits = digits != "";
        for (int i = 0; i < (int)digits.size(); i++) {
            if (!isdigit(digits[i])) allDigits = false;
        }
        if (allDigits && stoi(digits) >= nextId) {
            nextId = stoi(digits) + 1;
        }
    }

    return new Enrolment(id, studentId, courseId, date);
}
