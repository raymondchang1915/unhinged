#ifndef ATTENDANCERECORD_H
#define ATTENDANCERECORD_H
#include <iostream>
#include <string>
using namespace std;


//unique to a specific student id and a session id
// a data class that store data . 
class AttendanceRecord {
protected:   // protected so CorrectionRecord can read these directly
    string studentId;
    string sessionId;
    string timestamp;
    string status;       // "Present" or "Absent"
    string capturedBy;   // how it was captured: CardTap, ConsoleTap, Manual

public:
    AttendanceRecord(string stuId, string sessId, string ts, string stat, string capBy);
    virtual ~AttendanceRecord() = default;

    string getStudentId() const;
    string getSessionId() const;
    string getTimestamp() const;
    string getStatus() const;
    string getCapturedBy() const;

    // virtual copy: a session can copy its records without knowing which kind each one is
    virtual AttendanceRecord* clone() const; //needs virtual since it copies either parent or child classes

    virtual string toLine() const;
    static AttendanceRecord* fromLine(string line); //made static so the function can be called from the class blueprint without the existence f the object

    friend ostream& operator<<(ostream& os, const AttendanceRecord& r);
};

#endif
