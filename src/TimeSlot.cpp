#include "TimeSlot.h"
#include <iomanip> //for formatting input & output streams
#include <sstream> //for reading & writing strings
#include <iostream>

TimeSlot::TimeSlot() : day(1), startMin(540), endMin(660), location("Main Hall") {}

TimeSlot::TimeSlot(int d, int start, int end, std::string loc){
    day=d;
    startMin=start;
    endMin=end;
    location=loc;
}

int TimeSlot::getDay() const { return day; }
int TimeSlot::getStartMin() const { return startMin; }
int TimeSlot::getEndMin() const { return endMin; }
std::string TimeSlot::getLocation() const { return location; }

bool TimeSlot::overlaps(const TimeSlot& other) const {
    //check if they are on different days
    if (day!=other.day){
        return false;
    }

    //if on the same date
    if (startMin<other.endMin && other.startMin<endMin){
        return true; //clashes
    }
    return false;
}

//calls the overlaps function
bool TimeSlot::operator&&(const TimeSlot& other) const {
    return this->overlaps(other);
}

std::string TimeSlot::toLine() const {
    return std::to_string(day) + ";" + std::to_string(startMin) + ";" + std::to_string(endMin) + ";" + location;
}

TimeSlot TimeSlot::fromLine(const std::string& line) {
    std::stringstream ss(line);
    std::string dayStr, startStr, endStr, loc;
    std::getline(ss, dayStr, ';');
    std::getline(ss, startStr, ';');
    std::getline(ss, endStr, ';');
    std::getline(ss, loc);
    int d = dayStr.empty() ? 1 : std::stoi(dayStr);
    int s = startStr.empty() ? 540 : std::stoi(startStr);
    int e = endStr.empty() ? 660 : std::stoi(endStr);
    return TimeSlot(d, s, e, loc);
}

//prints the data to the screen
std::ostream& operator<<(std::ostream& os, const TimeSlot& slot){
    const char* days[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
    std::string dayName = (slot.day >= 0 && slot.day <= 6) ? days[slot.day] : ("Day " + std::to_string(slot.day));

    int sh = slot.startMin / 60;
    int sm = slot.startMin % 60;
    int eh = slot.endMin / 60;
    int em = slot.endMin % 60;

    os << dayName << " ";
    if (sh < 10) os << "0";
    os << sh << ":";
    if (sm < 10) os << "0";
    os << sm << " - ";
    if (eh < 10) os << "0";
    os << eh << ":";
    if (em < 10) os << "0";
    os << em << " (" << slot.location << ")";
    return os;
}