#ifndef ADMINISTRATOR_H
#define ADMINISTRATOR_H

#include "Person.h"
#include <string>
using namespace std;

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

    void generateEnrolmentReport();

    string toLine() const override;
};

#endif