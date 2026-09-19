#include "Administrator.h"
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
    cout << "removeUser not implemented yet: " << userId << "\n";
}

void Administrator::createCourse() {
    // needs Course + Repository<Course>
    cout << "createCourse not implemented yet\n";
}

void Administrator::editCourse(string courseCode) {
    cout << "editCourse not implemented yet: " << courseCode << "\n";
}

void Administrator::removeCourse(string courseCode) {
    cout << "removeCourse not implemented yet: " << courseCode << "\n";
}

void Administrator::generateEnrolmentReport() {
    // FR6.1 - needs Course + Enrolment
    cout << "generateEnrolmentReport not implemented yet\n";
}

string Administrator::toLine() const {
    return "ADM|" + getId() + "|" + getName();
}