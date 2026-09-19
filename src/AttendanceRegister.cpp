#include "AttendanceRegister.h"

#include <fstream>
#include <iomanip>
#include <iostream>
using namespace std;

AttendanceSession& AttendanceRegister::openSession(string courseId, TimeSlot slot, int durationMin) {
    string sessId = courseId + "_SESS_" + to_string(sessions.size() + 1);
    sessions.push_back(AttendanceSession(sessId, courseId, slot, "2026-09-17 10:00", durationMin));
    cout << "Successfully opened attendance session: " << sessId
         << " for course " << courseId << " (duration: " << durationMin << " mins)\n";
    return sessions.back();
}

// newest first, so the session opened most recently wins
AttendanceSession* AttendanceRegister::getActiveSession(string courseId) {
    for (int i = (int)sessions.size() - 1; i >= 0; i--) {
        if (sessions[i].getCourseId() == courseId && !sessions[i].isExpired()) {
            return &sessions[i];
        }
    }
    return nullptr;
}

AttendanceSession* AttendanceRegister::getActiveSession() {
    for (int i = (int)sessions.size() - 1; i >= 0; i--) {
        if (!sessions[i].isExpired()) {
            return &sessions[i];
        }
    }
    return nullptr;
}

AttendanceSession* AttendanceRegister::findSessionById(string sessionId) {
    for (int i = 0; i < (int)sessions.size(); i++) {
        if (sessions[i].getId() == sessionId) {
            return &sessions[i];
        }
    }
    return nullptr;
}

// the shared tail of both processEvent overloads: find the open session, reject
// a second tap from the same student, then write the record
void AttendanceRegister::recordTap(const string& studentId, const string& capturedBy) {
    AttendanceSession* session = getActiveSession();
    if (!session) {
        throw SessionClosedException();
    }

    for (AttendanceRecord* rec : session->getRecords()) {
        if (rec && rec->getStudentId() == studentId) {
            throw DuplicateAttendanceException();
        }
    }

    session->markPresent(studentId, capturedBy);
}

// card tap: the UID on the card is already the student ID
void AttendanceRegister::processEvent(CaptureEvent event) {
    recordTap(event.getUid(), "CardTap");
}

// console tap: the caller supplies the UID lookup and the enrolment check,
// because the register does not know about students or courses
void AttendanceRegister::processEvent(CaptureEvent event,
                                      const function<string(const string&)>& uidToStudentId,
                                      const function<bool(const string&, const string&)>& isStudentEnrolled) {
    AttendanceSession* session = getActiveSession();
    if (!session) {
        throw SessionClosedException();
    }

    string rawUid = event.getUid();
    string studentId = uidToStudentId ? uidToStudentId(rawUid) : rawUid;

    if (isStudentEnrolled && !isStudentEnrolled(studentId, session->getCourseId())) {
        throw NotEnrolledException();
    }

    recordTap(studentId, "ConsoleTap");
}

int AttendanceRegister::countSessionsFor(const string& courseId) const {
    int total = 0;
    for (const AttendanceSession& sess : sessions) {
        if (sess.getCourseId() == courseId) {
            total++;
        }
    }
    return total;
}

// percentage of this course's sessions where the student has a record
double AttendanceRegister::attendancePercent(string studentId, string courseId) const {
    int total = countSessionsFor(courseId);
    if (total == 0) {
        return 100.0;   // no sessions held yet, so nobody can be behind
    }

    int attended = 0;
    for (const AttendanceSession& sess : sessions) {
        if (sess.getCourseId() != courseId) {
            continue;
        }
        for (AttendanceRecord* rec : sess.getRecords()) {
            if (rec && rec->getStudentId() == studentId) {
                attended++;
                break;
            }
        }
    }

    return ((double)attended / total) * 100.0;
}

void AttendanceRegister::courseAttendanceReport(string courseId) const {
    int totalSessions = countSessionsFor(courseId);

    cout << "\n========================================================\n";
    cout << "         ATTENDANCE REPORT FOR COURSE: " << courseId << "\n";
    cout << "========================================================\n";
    cout << " Total Sessions Held: " << totalSessions << "\n";

    if (totalSessions == 0) {
        cout << " No attendance sessions recorded for this course yet.\n";
        cout << "========================================================\n\n";
        return;
    }

    cout << "\n Sessions breakdown:\n";
    for (const AttendanceSession& sess : sessions) {
        if (sess.getCourseId() != courseId) {
            continue;
        }
        cout << "  - Session: " << sess.getId() << " (Opened: " << sess.getOpenedAt()
             << ", Status: " << (sess.getIsOpen() ? "OPEN" : "CLOSED") << ")\n";
        cout << "    Attendees (" << sess.getRecords().size() << "):\n";
        for (AttendanceRecord* rec : sess.getRecords()) {
            if (rec) {
                cout << "      * " << *rec << "\n";
            }
        }
    }
    cout << "========================================================\n\n";
}

// same report, but one row per enrolled student with the 80% eligibility rule
void AttendanceRegister::courseAttendanceReport(string courseId, const vector<string>& enrolledStudents) const {
    int totalSessions = countSessionsFor(courseId);

    cout << "\n========================================================\n";
    cout << "       DETAILED ATTENDANCE REPORT: " << courseId << "\n";
    cout << "========================================================\n";
    cout << " Total Sessions Held: " << totalSessions << "\n";
    cout << " Enrolled Students:   " << enrolledStudents.size() << "\n";
    cout << "--------------------------------------------------------\n";
    cout << left << setw(15) << "Student ID" << setw(20) << "Attendance %" << "Status\n";
    cout << "--------------------------------------------------------\n";

    double totalPct = 0;
    for (const string& stuId : enrolledStudents) {
        double pct = attendancePercent(stuId, courseId);
        totalPct += pct;

        cout << left << setw(15) << stuId << setw(18) << (to_string((int)pct) + "%");
        if (pct < 80.0 && totalSessions > 0) {
            cout << " [!] Ineligible (<80% threshold)\n";
        } else {
            cout << " [OK] Eligible\n";
        }
    }

    if (!enrolledStudents.empty()) {
        cout << "--------------------------------------------------------\n";
        cout << " Overall Course Attendance Average: "
             << fixed << setprecision(1) << (totalPct / enrolledStudents.size()) << "%\n";
    }
    cout << "========================================================\n\n";
}

// attendance.txt holds a SESSION line followed by that session's record lines
void AttendanceRegister::save(const string& path) const {
    ofstream file(path);
    if (!file.is_open()) {
        throw PersistenceException("Error: Could not save attendance file: " + path);
    }

    for (const AttendanceSession& sess : sessions) {
        file << "SESSION," << sess.toLine() << "\n";
        for (AttendanceRecord* rec : sess.getRecords()) {
            if (rec) {
                file << rec->toLine() << "\n";
            }
        }
    }
    file.close();
}

void AttendanceRegister::load(const string& path) {
    ifstream file(path);
    if (!file.is_open()) {
        return;   // no file yet, nothing to load
    }

    sessions.clear();
    string line;
    while (getline(file, line)) {
        if (line == "") {
            continue;
        }

        if (line.rfind("SESSION,", 0) == 0) {
            AttendanceSession* sess = AttendanceSession::fromLine(line.substr(8));
            if (sess) {
                sessions.push_back(*sess);
                delete sess;
            }
            continue;
        }

        bool isRecord = (line.rfind("RECORD,", 0) == 0 || line.rfind("CORRECTION,", 0) == 0);
        if (!isRecord || sessions.empty()) {
            continue;
        }

        AttendanceRecord* rec = AttendanceRecord::fromLine(line);
        if (!rec) {
            continue;
        }

        // put the record back on its own session, or the newest one if the ID is unknown
        AttendanceSession* target = findSessionById(rec->getSessionId());
        if (!target) {
            target = &sessions.back();
        }

        // notify = false: this is a reload, not a live tap, so stay quiet
        CorrectionRecord* correction = dynamic_cast<CorrectionRecord*>(rec);
        if (correction) {
            target->appendCorrection(correction->getStudentId(), correction->getActingLecturerId(),
                                     correction->getReason(), false);
        } else {
            target->markPresent(rec->getStudentId(), rec->getCapturedBy(), false);
        }
        delete rec;
    }
    file.close();
}
