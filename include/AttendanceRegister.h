#ifndef ATTENDANCEREGISTER_H
#define ATTENDANCEREGISTER_H

#include "AttendanceSession.h"
#include "CaptureEvent.h"
#include "Exceptions.h"
#include "TimeSlot.h"

#include <string>
#include <vector>
using namespace std;

// owns every attendance session and turns card taps into records
class AttendanceRegister {
private:
    vector<AttendanceSession> sessions;

    int countSessionsFor(const string& courseId) const;   // shared by both reports

public:
    AttendanceRegister();

    AttendanceSession& openSession(string courseId, TimeSlot slot, int durationMin = 10);
    AttendanceSession* getActiveSession(string courseId);
    AttendanceSession* getActiveSession();
    AttendanceSession* findSessionById(string sessionId);

    vector<AttendanceSession>& getAllSessions();
    const vector<AttendanceSession>& getAllSessions() const;

    void processEvent(CaptureEvent event);   // one tap: resolve the UID, check, record

    double attendancePercent(string studentId, string courseId) const;
    void courseAttendanceReport(string courseId) const;

    void save(const string& path) const;
    void load(const string& path);
};

#endif
