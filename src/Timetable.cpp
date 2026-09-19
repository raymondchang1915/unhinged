#include "Timetable.h"

// FR4.4: Checks clash detection between slots using operator&&
bool Timetable::hasClash(const TimeSlot& newSlot) const {
    for (const auto& existing : slots) {
        if (existing && newSlot) { // Calls TimeSlot::operator&&
            return true;
        }
    }
    return false;
}

void Timetable::addSlot(const TimeSlot& slot) {
    addSlot(slot, "");
}

void Timetable::addSlot(const TimeSlot& slot, const std::string& courseId) {
    slots.push_back(slot);
    slotCourses.push_back(courseId);
}

// drops every slot that came from one course, used when a student drops it
void Timetable::removeSlotsOf(const std::string& courseId) {
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

const std::vector<TimeSlot>& Timetable::getSlots() const {
    return slots;
}

// FR6.2: Overloaded stream insertion operator for Timetable
std::ostream& operator<<(std::ostream& os, const Timetable& tt) {
    if (tt.slots.empty()) {
        os<< "  (No scheduled classes in timetable)\n";
        return os;
    }
    for (size_t i = 0; i < tt.slots.size(); ++i) {
        os<< "  Slot "<<(i + 1)<<": "<<tt.slots[i]<<"\n";
    }
    return os;
}
