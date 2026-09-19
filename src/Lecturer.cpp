#include "Lecturer.h"
#include "UniversitySystem.h"
#include "Course.h"
#include "Exceptions.h"

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

// FR2.3: a lecturer may only touch courses assigned to them
void Lecturer::openSession(string courseId, const TimeSlot& slot, int durationMin) {
    UniversitySystem& sys = UniversitySystem::getInstance();
    Course* course = sys.findCourse(courseId);
    if (!course) {
        throw SystemException("Course '" + courseId + "' not found.");
    }
    if (course->getAssignedLectId() != getId()) {
        throw SystemException("Course " + courseId + " is not assigned to you.");
    }
    sys.getAttendance().openSession(courseId, slot, durationMin);
}

void Lecturer::closeSession(string sessionId) {
    AttendanceSession* session = UniversitySystem::getInstance().getAttendance().findSessionById(sessionId);
    if (!session) {
        throw SystemException("Session '" + sessionId + "' not found.");
    }
    session->close();
}

void Lecturer::viewEnrolmentList(string courseId) {
    UniversitySystem& sys = UniversitySystem::getInstance();
    Course* course = sys.findCourse(courseId);
    if (!course) {
        throw SystemException("Course '" + courseId + "' not found.");
    }
    if (course->getAssignedLectId() != getId()) {
        throw SystemException("Course " + courseId + " is not assigned to you.");
    }

    cout << "\n--- Enrolment list for " << *course << " ---\n";
    vector<string> enrolled = course->getEnrolledIds();
    if (enrolled.empty()) {
        cout << "  (nobody enrolled yet)\n";
        return;
    }
    for (int i = 0; i < (int)enrolled.size(); i++) {
        Person* p = sys.getUsers().findById(enrolled[i]);
        cout << "  " << (i + 1) << ". " << enrolled[i];
        if (p) {
            cout << " - " << p->getName();
        }
        cout << "\n";
    }
}

string Lecturer::toLine() const {
    return "Lecturer," + getId() + "," + getName() + "," + getPassword();
}