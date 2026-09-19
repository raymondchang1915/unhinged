#include "Student.h"
#include <iostream>

Student::Student(string i, string h, string p, string u)
    : Person(i, h, p) {
    // TODO: u is the card number - pass it to StudentCard once M2 pushes that class
    (void)u;
}

Student::~Student(){}


void Student::showDashboard() {
    cout << "\n--- Student Dashboard ---\n";
    cout << "Logged in as: " << getName() << " (" << getId() << ")\n";
    cout << "1. Enrol in a course\n";
    cout << "2. Drop a course\n";
    cout << "3. View my timetable\n";
    cout << "4. View my attendance\n";
    cout << "5. Logout\n";
}

void Student::enrol(string courseId) {
    // TODO: needs Course lookup (M3) and Timetable::hasClash (M2)
    // check order: already enrolled -> prerequisites -> capacity -> clash
    cout << "enrol not implemented yet: " << courseId << "\n";
}

void Student::drop(string courseId) {
    // TODO: remove enrolment, then timetable.removeSlotsOf(courseId)
    cout << "drop not implemented yet: " << courseId << "\n";
}

string Student::toLine() const {
    return "STU|" + getId() + "|" + getName();
}