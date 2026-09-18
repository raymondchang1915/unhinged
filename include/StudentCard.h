#ifndef STUDENTCARD_H
#define STUDENTCARD_H
#include <string>

class StudentCard{

private:
    char* uid; //character pointer
public:
    StudentCard(); // default constructor
    StudentCard(std::string id);
    //Copy constructor
    StudentCard(const StudentCard& other);
    //copy asssignment operator
    StudentCard& operator=(const StudentCard& other);
    //Move constructor
    StudentCard(StudentCard&& other) noexcept;
    //Move assignment operator
    StudentCard& operator=(StudentCard&& other) noexcept;
    //Destructor
    ~StudentCard();

    std::string getUid() const;
    std::string toLine() const;
};
#endif