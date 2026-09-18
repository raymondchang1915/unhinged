#ifndef ATTENDANCESESSION_H
#define ATTENDANCESESSION_H

//headers
#include <string>
#include <vector>
#include "TimeSlot.h"
#include "AttendanceRecord.h"
#include "CorrectionRecord.h"

class AttendanceSession{
    private:
    static int defaultDurationMin;
    std::string id;
    std::string courseId;
    TimeSlot slot;
    std::string openedAt;
    int durationMin;
    bool isOpen;

    //Set as pointers in order to hold both normal records and correction records
    std::vector<AttendanceRecord*> records;

    public:
    AttendanceSession(std::string sessId,std::string courseId,TimeSlot slot,std::string timeOpened,int duration = 10);

    //Rule of 5 implemented
    AttendanceSession(const AttendanceSession& other);
    AttendanceSession& operator=(const AttendanceSession& other);
    AttendanceSession(AttendanceSession&& other) noexcept;
    AttendanceSession& operator=(AttendanceSession&& other) noexcept;

    //Getter functions
    std::string getId() const;
    std::string getCourseId() const;
    TimeSlot getSlot() const;
    std::string getOpenedAt() const;
    int getDurationMin() const;
    bool getIsOpen() const;
    const std::vector<AttendanceRecord*>& getRecords() const;

    void close();
    bool isExpired() const;
    void markPresent(std::string studentId,std::string capturedBy, bool notify = true);
    void appendCorrection(std::string studentId,std::string lecturerId,std::string reason, bool notify = true);

    //Ease of saving files
    std::string toLine() const;
    static AttendanceSession* fromLine(std::string line);

    // Destructor to clean up
    ~AttendanceSession();

};
#endif