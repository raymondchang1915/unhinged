#include "Person.h"

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

bool Person::login(string pw) const {
    return password == pw; //returns true if the password and input pw match
}