#ifndef PERSON_H
#define PERSON_H

#include <string>
using namespace std;

class Person {
private:
    string id;
    string name;
    string password;

public:
//since the compiler oly looks at the type here
    Person(string , string , string );
    virtual ~Person();

    string getId() const;
    string getName() const;
    bool login(string pw) const;
//virtual function and equallling it to 0 makes this an abstract class
    virtual void showDashboard() = 0;   // each role draws its own menu
    virtual string toLine() const = 0;  // each role writes its own tag

    static Person* fromLine(string line);  //this reads a line and return an pointer to person object no need for a class to exist since it's static
};
 
#endif
