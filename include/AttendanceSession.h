#ifndef ATTENDANCESESSION_H
#define ATTENDANCESESSION_H

#include "AttendanceRecord.h"
#include "CorrectionRecord.h"
#include "TimeSlot.h"

#include <string>
#include <vector>
using namespace std;

// one class meeting that students tap into
class AttendanceSession {
private:
    static int defaultDurationMin;   // shared by every session: 10 minutes

    string id;
    string courseId;
    TimeSlot slot;
    string openedAt;
    int durationMin;
    bool isOpen;

    // pointers, so one vector can hold both AttendanceRecord and CorrectionRecord
    vector<AttendanceRecord*> records;

public:
    AttendanceSession(string sessId, string courseId, TimeSlot slot, string timeOpened, int duration = 10);

    // rule of 5: the records are owned pointers, so copying needs deep copies
    AttendanceSession(const AttendanceSession& other);
    AttendanceSession& operator=(const AttendanceSession& other);
    AttendanceSession(AttendanceSession&& other) noexcept;
    AttendanceSession& operator=(AttendanceSession&& other) noexcept;
    ~AttendanceSession();

    string getId() const;
    string getCourseId() const;
    TimeSlot getSlot() const;
    string getOpenedAt() const;
    int getDurationMin() const;
    bool getIsOpen() const;
    const vector<AttendanceRecord*>& getRecords() const;

    void close();
    bool isExpired() const;
    void markPresent(string studentId, string capturedBy, bool notify = true);
    void appendCorrection(string studentId, string lecturerId, string reason, bool notify = true);

    string toLine() const;
    static AttendanceSession* fromLine(string line);
};

#endif
