#ifndef TIMESLOT_H
#define TIMESLOT_H
#include <string>
#include <iostream>

class TimeSlot {

private:
    int day;
    int startMin;
    int endMin;
    std::string location;

public:
    TimeSlot();
    TimeSlot(int d, int start, int end, std::string loc); //constructor
    int getDay() const;
    int getStartMin() const;
    int getEndMin() const;
    std::string getLocation() const;

    bool overlaps(const TimeSlot& other) const; //checks if another time slot happens at the same time
    bool operator&&(const TimeSlot& other) const;

    std::string toLine() const;
    static TimeSlot fromLine(const std::string& line);

    friend std::ostream& operator<<(std::ostream& os, const TimeSlot& slot); //gives print function acces to look at private data
};
#endif