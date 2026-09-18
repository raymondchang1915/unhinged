#include "StudentCard.h"
#include <cstring>
using namespace std;
StudentCard::StudentCard() {
    uid = new char[1];
    uid[0] = '\0';
}

StudentCard::StudentCard(string id){//allocating memory
    uid=new char[id.length()+1]; //plus 1 for the hidden null terminator
    //copying the text from the id string to the new memory space
    strcpy(uid,id.c_str());
}
//copy constructor
StudentCard::StudentCard(const StudentCard& other){
    if (other.uid) {
        uid=new char[strlen(other.uid)+1];
        strcpy(uid,other.uid);
    } else {
        uid=new char[1];
        uid[0] = '\0';
    }
}

//copy assignment operator
StudentCard& StudentCard::operator=(const StudentCard& other){
    if (this!=&other){
        delete[] uid;
        if (other.uid) {
            uid=new char[strlen(other.uid)+1];
            strcpy(uid,other.uid);
        } else {
            uid=new char[1];
            uid[0] = '\0';
        }
    }
    return *this;
}

//move constructor
StudentCard::StudentCard(StudentCard&& other) noexcept {
    uid = other.uid;
    other.uid = nullptr;
}

//move assignment operator
StudentCard& StudentCard::operator=(StudentCard&& other) noexcept {
    if (this != &other) {
        delete[] uid;
        uid = other.uid;
        other.uid = nullptr;
    }
    return *this;
}

//Destructor
StudentCard::~StudentCard(){
    delete[] uid;
}

string StudentCard::getUid() const{
    return uid ? string(uid) : string();
}
string StudentCard::toLine() const{
    return getUid();
}