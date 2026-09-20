#ifndef UNIVERSITYSYSTEM_H
#define UNIVERSITYSYSTEM_H

#include <string>
#include <vector>
#include "Repository.h"
#include "Person.h"
#include "Course.h"
#include "Enrolment.h"
#include "AttendanceRegister.h"

//Forward declarations
class Student;
class Lecturer;
class Administrator;

class UniversitySystem {
private://encapsulation
    std::string dataDir;//holds the folder name of data 
    Repository<Person> users;//for the template in the repository.h
    Repository<Course> courses;
    Repository<Enrolment> enrolments;
    AttendanceRegister attendance;

    static UniversitySystem* instance; //pointer shared by whole system the instace belongs to the class itself
    UniversitySystem();//no outside can create another new university system

public:
    static UniversitySystem& getInstance();//how outside get access to the system if the system doesnt exists it creates one
//with & gices a direct view to the repos without making copies
    Repository<Person>& getUsers() { return users; }
    Repository<Course>& getCourses() { return courses; }//functions to let other see the repos
    AttendanceRegister& getAttendance() { return attendance; }

    void initialize(const std::string& dir = "data");//default folder "data"
    void loadAll();//functions to load save and start the system
    void saveAll();
    //pass by reference '&' so that the string is given to functions without making a copy
    //const used so no change can occur
    Student* findStudent(const std::string& id);//pointer return used so that its fast and modifies the real object and not found can happen
    Course* findCourse(const std::string& code);//& prevents copies of the string

    std::vector<Enrolment*> getEnrolmentsForStudent(const std::string& studentId);
    std::string resolveUidToStudentId(const std::string& uid);

    void enrolStudent(const std::string& studentId, const std::string& courseCode);//functions to enrol or drop
    void dropStudent(const std::string& studentId, const std::string& courseCode);
    void rebuildStudentTimetables();
};

#endif
