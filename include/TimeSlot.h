#ifndef TIMESLOT_H
#define TIMESLOT_H

#include <iostream>
#include <string>
using namespace std;

// one scheduled block of time: which day, from when to when, and where
class TimeSlot {
private:
    int day;        // 0 = Sunday, 1 = Monday, and so on
    int startMin;   // minutes from midnight, so 540 means 09:00
    int endMin;
    string location;

public:
    TimeSlot();
    TimeSlot(int d, int start, int end, string loc);

    int getDay() const;
    int getStartMin() const;
    int getEndMin() const;
    string getLocation() const;

    bool overlaps(const TimeSlot& other) const;      // same day and the times cross
    bool operator&&(const TimeSlot& other) const;    // reads as "slotA && slotB"

    string toLine() const;
    static TimeSlot fromLine(const string& line);

    friend ostream& operator<<(ostream& os, const TimeSlot& slot);
};

#endif
