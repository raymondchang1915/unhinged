#include "Lecturer.h"
#include <iostream>

Lecturer::Lecturer(string i, string h, string p) : Person(i, h, p) {
}

Lecturer::~Lecturer() {
}

void Lecturer::showDashboard() {
    cout << "\n--- Lecturer Dashboard ---\n";
    cout << "Logged in as: " << getName() << " (" << getId() << ")\n";
    cout << "1. View my courses\n";
    cout << "2. View enrolment list\n";
    cout << "3. Open attendance session\n";
    cout << "4. Close attendance session\n";
    cout << "5. Attendance report\n";
    cout << "6. Logout\n";
}

void Lecturer::openSession(string courseId, const TimeSlot& slot, int durationMin) {
    // needs AttendanceRegister (M2)
    cout << "openSession not implemented yet: " << courseId << "\n";
}

void Lecturer::closeSession(string sessionId) {
    // needs AttendanceRegister (M2)
    cout << "closeSession not implemented yet: " << sessionId << "\n";
}

void Lecturer::viewEnrolmentList(string courseId) {
    // needs Course + repository lookup; must reject courses not assigned to this lecturer (FR2.3)
    cout << "viewEnrolmentList not implemented yet: " << courseId << "\n";
}

string Lecturer::toLine() const {
    return "LEC|" + getId() + "|" + getName();
}