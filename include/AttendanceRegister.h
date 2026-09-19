#ifndef ATTENDANCEREGISTER_H
#define ATTENDANCEREGISTER_H

#include <string>
#include <vector>
#include <functional>
#include "TimeSlot.h"
#include "AttendanceSession.h"
#include "CaptureEvent.h"

#include "Exceptions.h"

class AttendanceRegister {
    private:
    std::vector<AttendanceSession> sessions;

    // shared by both processEvent overloads and both report overloads
    void recordTap(const std::string& studentId, const std::string& capturedBy);
    int countSessionsFor(const std::string& courseId) const;

    public:
    AttendanceRegister() {}
    AttendanceSession& openSession(std::string courseId,TimeSlot slot,int durationMin = 10);
    AttendanceSession* getActiveSession(std::string courseId);
    AttendanceSession* getActiveSession();
    AttendanceSession* findSessionById(std::string sessionId);
    std::vector<AttendanceSession>& getAllSessions() { return sessions; }
    const std::vector<AttendanceSession>& getAllSessions() const { return sessions; }

    void processEvent(CaptureEvent event);
    void processEvent(CaptureEvent event, const std::function<std::string(const std::string&)>& uidToStudentId,
                      const std::function<bool(const std::string&, const std::string&)>& isStudentEnrolled);

    double attendancePercent(std::string studentId,std::string courseId) const;
    void courseAttendanceReport(std::string courseId) const;
    void courseAttendanceReport(std::string courseId, const std::vector<std::string>& enrolledStudents) const;

    void save(const std::string& path) const;
    void load(const std::string& path);
};

#endif