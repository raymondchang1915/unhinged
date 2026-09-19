#ifndef COURSE_H
#define COURSE_H

#include <iostream>
#include <string>
#include <vector>
using namespace std;

class TimeSlot;

class Course {
private:
    string code;
    string title;
    int creditVal;
    int capacity;
    string assignedLectId;
    vector<string> prereq;
    vector<string> enrolledIds;
    // vector<TimeSlot> slots;   // uncomment once M2 pushes TimeSlot.h

public:
    Course(string code, string title, int creditVal, int capacity, string lectId);
    virtual ~Course();

    string getId() const;
    string getTitle() const;
    int getCreditVal() const;
    int getCapacity() const;
    string getAssignedLectId() const;
    vector<string> getPrereq() const;
    vector<string> getEnrolledIds() const;

    bool isFull() const;
    void addStudent(string studentId);
    void removeStudent(string studentId);
    void addPrereq(string courseCode);

    virtual string toLine() const;
    static Course* fromLine(string line);

    virtual int calculateCredits() const = 0;
};

ostream& operator<<(ostream& os, const Course& c);

#endif
