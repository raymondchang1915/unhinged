#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <exception>
#include <string>
using namespace std;

// base exception 
class SystemException : public exception {
private:
    string message;   // error msg shown to the user

public:
    SystemException(string msg);
    const char* what() const noexcept override;
    //strings are not use so character arrays are used here what is to return the messge inside the exception and noexcept is used when an exception is traveling up the stack it won't make another
    //override is used toavoid ttypos and see if the function matches the base
    virtual ~SystemException();
};

//user exceptions
class UserException : public SystemException {
public:
    UserException(string msg);
};

class UserNotFoundException : public UserException {
private:
    string userId;
public:
    UserNotFoundException(string userId);
    string getUserId() const;
};

class InvalidCredentialsException : public UserException {
public:
    InvalidCredentialsException();
};

// enrollment exceptions
class EnrolmentException : public SystemException {
public:
    EnrolmentException(string msg);
};

class PrerequisiteNotMetException : public EnrolmentException {
private:
    string courseCode;
    string missingPrereq;
public:
    PrerequisiteNotMetException(string courseCode, string missingPrereq);
    string getCourseCode() const;
    string getMissingPrereq() const;
};

class CourseFullException : public EnrolmentException {
private:
    string courseCode;
    int capacity;
public:
    CourseFullException(string courseCode, int capacity);
    string getCourseCode() const;
    int getCapacity() const;
};

class ClashException : public EnrolmentException {
private:
    string slotA;   // slot descriptions, not TimeSlot objects
    string slotB;
public:
    ClashException(string slotA, string slotB);
    string getSlotA() const;
    string getSlotB() const;
};

class AlreadyEnrolledException : public EnrolmentException {
private:
    string courseCode;
public:
    AlreadyEnrolledException(string courseCode);
    string getCourseCode() const;
};

//  attendance exceptions 
class AttendanceException : public SystemException {
public:
    AttendanceException(string msg);
};

class SessionClosedException : public AttendanceException {
public:
    SessionClosedException();
};

class NotEnrolledException : public AttendanceException {
public:
    NotEnrolledException();
};

class DuplicateAttendanceException : public AttendanceException {
public:
    DuplicateAttendanceException();
};

//  persistence exceptions
class PersistenceException : public SystemException {
public:
    PersistenceException(string msg);
};

class CorruptDataException : public PersistenceException {
private:
    string filePath;
    int lineNo;
public:
    CorruptDataException(string filePath, int lineNo);
    string getFilePath() const;
    int getLineNo() const;
};

class FileNotFoundException : public PersistenceException {
private:
    string filePath;
public:
    FileNotFoundException(string filePath);
    string getFilePath() const;
};

#endif