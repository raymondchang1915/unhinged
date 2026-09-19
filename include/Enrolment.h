#ifndef ENROLMENT_H
#define ENROLMENT_H

#include <iostream>
#include <string>
using namespace std;

class Enrolment {
private:
    string id;
    string studentId;
    string courseId;
    string enrolmentDate;

    static int nextId;   // shared by every Enrolment, defined in Enrolment.cpp

public:
    Enrolment(string sid, string cid, string date);            // new: ID is generated
    Enrolment(string i, string sid, string cid, string date);  // loaded: ID comes from the file

    string getId() const;
    string getStudentId() const;
    string getCourseId() const;
    string getEnrolmentDate() const;

    string toLine() const;

    bool operator<(const Enrolment& other) const;   // lets a vector of enrolments be sorted
};

ostream& operator<<(ostream& os, const Enrolment& e);

#endif
