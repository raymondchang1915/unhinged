#ifndef LECTURER_H
#define LECTURER_H

#include "Person.h"
#include <string>
using namespace std;

class TimeSlot;   // forward declaration - M2's class, full definition not needed here

class Lecturer : public Person {
public:
    Lecturer(string i, string h, string p);
    ~Lecturer();

    void showDashboard() override;
    void listMyCourses();   // only the courses assigned to this lecturer
    void openSession(string courseId, const TimeSlot& slot, int durationMin);
    void closeSession(string sessionId);
    void viewEnrolmentList(string courseId);

    string toLine() const override;
};

#endif