#include "Person.h"
#include "Student.h"
#include "Lecturer.h"
#include "Administrator.h"
#include "Exceptions.h"

#include <sstream>

Person::Person(string i, string n, string p) : id(i) , name(n) , password(p) {

}

Person::~Person() {
}

string Person::getId() const {
    return id;
}

string Person::getName() const {
    return name;
}

string Person::getPassword() const {
    return password;
}

bool Person::login(string pw) const {
    return password == pw; //returns true if the password and input pw match
}

// users.txt format:  Role,id,name,password[,completedCourses]
// Role is Student, Lecturer or Admin. Completed courses are ';' separated, or NONE.
Person* Person::fromLine(string line) {
    stringstream ss(line);
    string role, id, name, password, completed;

    getline(ss, role, ',');
    getline(ss, id, ',');
    getline(ss, name, ',');
    getline(ss, password, ',');
    getline(ss, completed);

    if (role == "Student") {
        // the card UID is not stored in users.txt, it is derived from the ID
        Student* s = new Student(id, name, password, "CARD_" + id);
        if (completed != "" && completed != "NONE") {
            stringstream cs(completed);
            string code;
            while (getline(cs, code, ';')) {
                if (code != "") s->addCompletedCourse(code);
            }
        }
        return s;
    }
    if (role == "Lecturer") {
        return new Lecturer(id, name, password);
    }
    if (role == "Admin") {
        return new Administrator(id, name, password);
    }

    throw CorruptDataException("users.txt", 0);
}
