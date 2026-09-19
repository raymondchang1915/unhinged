#ifndef COURSE_H
#define COURSE_H

#include "TimeSlot.h"

#include <iostream>
#include <string>
#include <vector>
using namespace std;

class Course {
private:
    string code;
    string title;
    int creditVal;
    int capacity;
    string assignedLectId;
    vector<string> prereq;
    vector<string> enrolledIds;
    vector<TimeSlot> slots;

public:
    Course(string code, string title, int creditVal, int capacity, string lectId);
    virtual ~Course();

    string getId() const;
    string getTitle() const;
    int getCreditVal() const;
    int getCapacity() const;
    string getAssignedLectId() const;
    vector<string> getPrerequisites() const;
    vector<string> getEnrolledIds() const;
    const vector<TimeSlot>& getSlots() const;

    bool isFull() const;
    void addStudent(string studentId);
    void removeStudent(string studentId);
    void addPrereq(string courseCode);
    void addSlot(const TimeSlot& slot);

    // the two variable-length fields of a courses.txt line, used by the subclasses' toLine()
    string prereqToLine() const;
    string slotsToLine() const;

    virtual string toLine() const;
    static Course* fromLine(string line);

    virtual int calculateCredits() const = 0;
};

ostream& operator<<(ostream& os, const Course& c);

#endif
