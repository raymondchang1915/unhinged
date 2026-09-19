#include "Administrator.h"
#include "UniversitySystem.h"
#include "Course.h"
#include "Enrolment.h"
#include "Exceptions.h"

#include <iostream>

Administrator::Administrator(string i, string h, string p) : Person(i, h, p) {
}

Administrator::~Administrator() {
}

void Administrator::showDashboard() {
    cout << "\n--- Administrator Dashboard ---\n";
    cout << "Logged in as: " << getName() << " (" << getId() << ")\n";
    cout << "1. Manage users\n";
    cout << "2. Manage courses\n";
    cout << "3. Enrolment report\n";
    cout << "4. Logout\n";
}

void Administrator::createUser() {
    // needs Repository<Person> (M3)
    cout << "createUser not implemented yet\n";
}

void Administrator::updateUser(string userId) {
    cout << "updateUser not implemented yet: " << userId << "\n";
}

void Administrator::removeUser(string userId) {
    UniversitySystem& sys = UniversitySystem::getInstance();
    if (!sys.getUsers().findById(userId)) {
        throw UserNotFoundException(userId);
    }
    sys.getUsers().remove(userId);
    sys.saveAll();
    cout << "[Success] Removed user " << userId << "\n";
}

void Administrator::createCourse() {
    // needs Course + Repository<Course>
    cout << "createCourse not implemented yet\n";
}

void Administrator::editCourse(string courseCode) {
    cout << "editCourse not implemented yet: " << courseCode << "\n";
}

void Administrator::removeCourse(string courseCode) {
    UniversitySystem& sys = UniversitySystem::getInstance();
    if (!sys.findCourse(courseCode)) {
        throw SystemException("Course '" + courseCode + "' not found.");
    }
    sys.getCourses().remove(courseCode);
    sys.saveAll();
    cout << "[Success] Removed course " << courseCode << "\n";
}

// FR6.1: every course with its roll, using Course's operator<<
void Administrator::generateEnrolmentReport() {
    UniversitySystem& sys = UniversitySystem::getInstance();

    cout << "\n========================================================\n";
    cout << "                  ENROLMENT REPORT\n";
    cout << "========================================================\n";

    for (Course* c : sys.getCourses().all()) {
        vector<string> enrolled = c->getEnrolledIds();
        cout << "\n" << *c << "\n";
        cout << "  Lecturer: " << c->getAssignedLectId()
             << " | Seats: " << enrolled.size() << "/" << c->getCapacity() << "\n";

        if (enrolled.empty()) {
            cout << "  (nobody enrolled)\n";
            continue;
        }
        for (const string& id : enrolled) {
            Person* p = sys.getUsers().findById(id);
            cout << "  - " << id << (p ? " (" + p->getName() + ")" : "") << "\n";
        }
    }
    cout << "========================================================\n\n";
}

string Administrator::toLine() const {
    return "Admin," + getId() + "," + getName() + "," + getPassword();
}