#ifndef ADMINISTRATOR_H
#define ADMINISTRATOR_H

#include "Person.h"
#include <string>
using namespace std;

class Course;   // only a pointer is needed in this header

class Administrator : public Person {
public:
    Administrator(string i, string h, string p);
    ~Administrator();

    void showDashboard() override;

    void createUser();
    void updateUser(string userId);
    void removeUser(string userId);

    void createCourse();
    void editCourse(string courseCode);
    void removeCourse(string courseCode);

    void listAllCourses();                        // every course in the system
    double reportCourseAttendance(Course* course);// one course's roll with each student's attendance %
    void generateEnrolmentReport();

    string toLine() const override;
};

#endif