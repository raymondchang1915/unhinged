#ifndef TIMETABLE_H
#define TIMETABLE_H

#include "TimeSlot.h"
#include <vector>
#include <string>
#include <iostream>

//timetable owns timeslot and manages the collection of scheduled timeslots for a student
class Timetable {
    private:
    std::vector<TimeSlot> slots;

    public:
    Timetable() = default;

    // Checks if adding a slot would clash with any existing slot
    bool hasClash(const TimeSlot& newSlot) const;

    void addSlot(const TimeSlot& slot); //adds slots

    void clear(); //clears slots

    const std::vector<TimeSlot>& getSlots() const; //returns slots

    // Stream insertion operator (FR6.2)
    friend std::ostream& operator<<(std::ostream& os, const Timetable& tt);
};

#endif