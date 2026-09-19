#include "Timetable.h"
using namespace std;

Timetable::Timetable() {
}

// FR4.4: clash detection, using TimeSlot's operator&&
bool Timetable::hasClash(const TimeSlot& newSlot) const {
    for (int i = 0; i < (int)slots.size(); i++) {
        if (slots[i] && newSlot) {
            return true;
        }
    }
    return false;
}
//in case course ID is forgotten
void Timetable::addSlot(const TimeSlot& slot) {
    addSlot(slot, "");
}

void Timetable::addSlot(const TimeSlot& slot, const string& courseId) {
    slots.push_back(slot);
    slotCourses.push_back(courseId);
}

// walks backwards so erasing does not skip the next slot
void Timetable::removeSlotsOf(const string& courseId) {
    for (int i = (int)slots.size() - 1; i >= 0; i--) {
        if (slotCourses[i] == courseId) {
            slots.erase(slots.begin() + i);
            slotCourses.erase(slotCourses.begin() + i);
        }
    }
}

void Timetable::clear() {
    slots.clear();
    slotCourses.clear();
}

const vector<TimeSlot>& Timetable::getSlots() const {
    return slots;
}

// FR6.2
ostream& operator<<(ostream& os, const Timetable& tt) {
    if (tt.slots.size() == 0) {
        os << "  (No scheduled classes in timetable)\n";
        return os;
    }
    for (int i = 0; i < (int)tt.slots.size(); i++) {
        os << "  Slot " << (i + 1) << ": " << tt.slots[i];
        if (tt.slotCourses[i] != "") {
            os << "  [" << tt.slotCourses[i] << "]";
        }
        os << "\n";
    }
    return os;
}
