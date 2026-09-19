#include "Student.h"
#include <iostream>

Student::Student(string i, string h, string p, string u)
    : Person(i, h, p), card(u) {
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
    // the real work lives in UniversitySystem::enrolStudent, which owns the repositories
    cout << "enrol not implemented yet: " << courseId << "\n";
}

void Student::drop(string courseId) {
    // the real work lives in UniversitySystem::dropStudent
    cout << "drop not implemented yet: " << courseId << "\n";
}

const StudentCard& Student::getCard() const {
    return card;
}

Timetable& Student::getTimetable() {
    return timetable;
}

const Timetable& Student::getTimetable() const {
    return timetable;
}

const vector<string>& Student::getCompletedCourses() const {
    return completedCourses;
}

void Student::addCompletedCourse(string courseCode) {
    completedCourses.push_back(courseCode);
}

// Student,id,name,password,completedCourses   (completed are ';' separated, NONE if empty)
string Student::toLine() const {
    string completed = "NONE";
    for (int i = 0; i < (int)completedCourses.size(); i++) {
        if (i == 0) {
            completed = completedCourses[i];
        } else {
            completed += ";" + completedCourses[i];
        }
    }
    return "Student," + getId() + "," + getName() + "," + getPassword() + "," + completed;
}
