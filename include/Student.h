#ifndef STUDENT_H
#define STUDENT_H

#include "Person.h"
#include "StudentCard.h"
#include "Timetable.h"
#include <string>
#include <vector>
using namespace std;

class Student : public Person {
private:
    vector<string> completedCourses;
    StudentCard card;
    Timetable timetable;

public:
    Student(string , string , string , string );
    ~Student();

    void showDashboard() override;
    void enrol(string courseId);
    void drop(string courseId);

    const StudentCard& getCard() const;
    Timetable& getTimetable();               // non-const: the system adds and clears slots
    const Timetable& getTimetable() const;
    const vector<string>& getCompletedCourses() const;
    void addCompletedCourse(string courseCode);

    string toLine() const override;
};

#endif
