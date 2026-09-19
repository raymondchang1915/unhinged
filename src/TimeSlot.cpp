#include "TimeSlot.h"

#include <sstream>
using namespace std;

TimeSlot::TimeSlot() : day(1), startMin(540), endMin(660), location("Main Hall") {
}

TimeSlot::TimeSlot(int d, int start, int end, string loc)
    : day(d), startMin(start), endMin(end), location(loc) {
}

int TimeSlot::getDay() const {
    return day;
}

int TimeSlot::getStartMin() const {
    return startMin;
}

int TimeSlot::getEndMin() const {
    return endMin;
}

string TimeSlot::getLocation() const {
    return location;
}

// two slots clash only if they are on the same day and their times cross
bool TimeSlot::overlaps(const TimeSlot& other) const {
    if (day != other.day) {
        return false;
    }
    if (startMin < other.endMin && other.startMin < endMin) {
        return true;
    }
    return false;
}

// && can be used to check if two timeslots overlap- operator based clash detection
bool TimeSlot::operator&&(const TimeSlot& other) const {
    return overlaps(other);
}

// day;start;end;location
string TimeSlot::toLine() const {
    return to_string(day) + ";" + to_string(startMin) + ";" + to_string(endMin) + ";" + location;
}
//return type and the function class is both timeslot
TimeSlot TimeSlot::fromLine(const string& line) {
    stringstream ss(line);
    string dayStr, startStr, endStr, loc;
//instead of getting the text. here arguemet ss show the line where it will be saved to daystr and it will be spplit after ;
    getline(ss, dayStr, ';');
    getline(ss, startStr, ';');
    getline(ss, endStr, ';');
    getline(ss, loc);

    int d = 1;
    int s = 540;
    int e = 660;
    if (dayStr != "") {
        d = stoi(dayStr);
    }
    if (startStr != "") {
        s = stoi(startStr);
    }
    if (endStr != "") {
        e = stoi(endStr);
    }

    return TimeSlot(d, s, e, loc);
}

// prints as "Mon 09:00 - 11:00 (Hall 1)"
//return a reference type ostream. arugemnets are the cout and the timslot referance . this allows it to print the timeslots directly using >>
ostream& operator<<(ostream& os, const TimeSlot& slot) {
    string dayName;
    if (slot.day == 0) {
        dayName = "Sun";
    } else if (slot.day == 1) {
        dayName = "Mon";
    } else if (slot.day == 2) {
        dayName = "Tue";
    } else if (slot.day == 3) {
        dayName = "Wed";
    } else if (slot.day == 4) {
        dayName = "Thu";
    } else if (slot.day == 5) {
        dayName = "Fri";
    } else if (slot.day == 6) {
        dayName = "Sat";
    } else {
        dayName = "Day " + to_string(slot.day);
    }

    int startHour = slot.startMin / 60;
    int startMinute = slot.startMin % 60;
    int endHour = slot.endMin / 60;
    int endMinute = slot.endMin % 60;

    os << dayName << " ";
    if (startHour < 10) {
        os << "0";
    }
    os << startHour << ":";
    if (startMinute < 10) {
        os << "0";
    }
    os << startMinute << " - ";
    if (endHour < 10) {
        os << "0";
    }
    os << endHour << ":";
    if (endMinute < 10) {
        os << "0";
    }
    os << endMinute << " (" << slot.location << ")";
    return os;
}
