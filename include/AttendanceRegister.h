#ifndef ATTENDANCEREGISTER_H
#define ATTENDANCEREGISTER_H

#include "AttendanceSession.h"
#include "CaptureEvent.h"
#include "Exceptions.h"
#include "TimeSlot.h"

#include <string>
#include <vector>
using namespace std;
//This has multiple attendance sessions that means multiple classes with a lot of students with unque UID and unque session ID.
// owns every attendance session and turns card taps into records
class AttendanceRegister {
private:
    vector<AttendanceSession> sessions; //a list of sessions is a register and each session is a list of records- each record is either correction or normal

    int countSessionsFor(const string& courseId) const;   // shared by both reports

public:
    AttendanceRegister();

    AttendanceSession& openSession(string courseId, TimeSlot slot, int durationMin = 10);
    AttendanceSession* getActiveSession(string courseId);
    AttendanceSession* getActiveSession();
    AttendanceSession* findSessionById(string sessionId);


    void processEvent(CaptureEvent event);   // one tap: resolve the UID, check, record

    double attendancePercent(string studentId, string courseId) const;
    void courseAttendanceReport(string courseId) const;

    void save(const string& path) const;
    void load(const string& path);
};

#endif
