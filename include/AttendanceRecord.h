#ifndef ATTENDANCERECORD_H
#define ATTENDANCERECORD_H

#include <string>
#include <iostream>

class AttendanceRecord{
    protected: //proteced so the correction record can get these

    std::string studentId;
    std::string sessionId;
    std::string timestamp;
    std::string status; //whether the student is present or not
    std::string capturedBy;

    public:
    //Constructor
    AttendanceRecord(std::string stuId,std::string sessId,std::string ts,std::string stat,std::string capBy);

    //destructor
    virtual ~AttendanceRecord()=default;

    //from previous data (getters)
    std::string getStudentId() const;
    std::string getSessionId() const;
    std::string getTimestamp() const;
    std::string getStatus() const;
    std::string getCapturedBy() const;

    //Virtual copy constructor creates perfect copy and hands out pointer
    virtual AttendanceRecord* clone() const;

    //compresses data into single comma separated string
    virtual std::string toLine() const;
    static AttendanceRecord* fromLine(std::string line);

    //otherwise cannot access protected data
    friend std::ostream& operator<<(std::ostream& os,const AttendanceRecord& r);
};
#endif