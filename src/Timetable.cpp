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
    slots.push_back(slot);
}

void Timetable::clear() {
    slots.clear();
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
