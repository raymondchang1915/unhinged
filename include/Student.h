#ifndef STUDENT_H
#define STUDENT_H

#include "Person.h"
//#include "StudentCard.h"   // comment out until M2 pushes
//#include "Timetable.h"     // comment out until M2 pushes
#include <string>
#include <vector>
using namespace std;

class Student : public Person {
private:
    vector<string> completedCourses;
// StudentCard card;        // comment out until M2 pushes
//Timetable timetable;     // comment out until M2 pushes

public:
    Student(string , string , string , string );
    ~Student();

    void showDashboard() override;
    void enrol(string courseId);
    void drop(string courseId);

    string toLine() const override;
};

#endif