#include "Exceptions.h"

#include <sstream>

// builds the clash message using TimeSlot's own operator<<
static string describeClash(const TimeSlot& a, const TimeSlot& b) {
    ostringstream os;
    os << "Error: Timetable clash between " << a << " and " << b << ".";
    return os.str();
}


// base

SystemException::SystemException(string msg) : message(msg) {}

const char* SystemException::what() const noexcept//extract the error messege
{
    return message.c_str();
}

SystemException::~SystemException() {}


// user exceptions

UserException::UserException(string msg) : SystemException(msg) {}


UserNotFoundException::UserNotFoundException(string userId): UserException("Error: User with ID " + userId + " was not found."), userId(userId) {}

string UserNotFoundException::getUserId() const
{
    return userId;
}


InvalidCredentialsException::InvalidCredentialsException(): UserException("Error: Invalid credentials. Incorrect password.") {}


// enrolment exceptions

EnrolmentException::EnrolmentException(string msg) : SystemException(msg) {}


PrerequisiteNotMetException::PrerequisiteNotMetException(string courseCode, string missingPrereq) : EnrolmentException("Error: Cannot enrol in " + courseCode + ". Missing prerequisite: " + missingPrereq + "."),courseCode(courseCode), missingPrereq(missingPrereq) {}

string PrerequisiteNotMetException::getCourseCode() const { return courseCode; }

string PrerequisiteNotMetException::getMissingPrereq() const { return missingPrereq; }


CourseFullException::CourseFullException(string courseCode, int capacity): EnrolmentException("Error: Course " + courseCode + " is full. Maximum capacity is " + to_string(capacity) + "."), courseCode(courseCode), capacity(capacity) {}

string CourseFullException::getCourseCode() const { return courseCode; }

int CourseFullException::getCapacity() const { return capacity; }


ClashException::ClashException(TimeSlot slotA, TimeSlot slotB) : EnrolmentException(describeClash(slotA, slotB)), slotA(slotA), slotB(slotB) {}

TimeSlot ClashException::getSlotA() const { return slotA; }

TimeSlot ClashException::getSlotB() const { return slotB; }



AlreadyEnrolledException::AlreadyEnrolledException(string courseCode) : EnrolmentException("Error: Student is already enrolled in " + courseCode + "."), courseCode(courseCode) {}

string AlreadyEnrolledException::getCourseCode() const { return courseCode; }



// attendance exceptions

AttendanceException::AttendanceException(string msg) : SystemException(msg) {}


SessionClosedException::SessionClosedException() : AttendanceException("Error: Attendance session has already been closed or expired.") {}


NotEnrolledException::NotEnrolledException() : AttendanceException("Error: Cannot mark attendance because student is not enrolled in this course.") {}


DuplicateAttendanceException::DuplicateAttendanceException() : AttendanceException("Error: Student has already marked attendance for this session.") {}


// persistence exceptions

PersistenceException::PersistenceException(string msg) : SystemException(msg) {}


CorruptDataException::CorruptDataException(string filePath, int lineNo) : PersistenceException("Error: Corrupt data found in file " + filePath + " at line " + to_string(lineNo) + "."), filePath(filePath), lineNo(lineNo) {}

string CorruptDataException::getFilePath() const { return filePath; }

int CorruptDataException::getLineNo() const { return lineNo; }


FileNotFoundException::FileNotFoundException(string filePath) : PersistenceException("Error: Could not locate or open file: " + filePath + "."), filePath(filePath) {}

string FileNotFoundException::getFilePath() const { return filePath; }

