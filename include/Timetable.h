#ifndef TIMETABLE_H
#define TIMETABLE_H

#include "TimeSlot.h"

#include <iostream>
#include <string>
#include <vector>
using namespace std;

// a student's weekly schedule: the slots they sit in, and which course each came from
class Timetable {
private:
    vector<TimeSlot> slots;
    vector<string> slotCourses;   // slotCourses[i] is the course that gave us slots[i]

public:
    Timetable();

    bool hasClash(const TimeSlot& newSlot) const;   // would adding this slot double-book them?

    void addSlot(const TimeSlot& slot);
    void addSlot(const TimeSlot& slot, const string& courseId);
    void removeSlotsOf(const string& courseId);     // used when a student drops a course
    void clear();

    const vector<TimeSlot>& getSlots() const;

    friend ostream& operator<<(ostream& os, const Timetable& tt);
};

#endif
