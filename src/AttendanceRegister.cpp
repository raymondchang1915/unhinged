#include "AttendanceRegister.h"
#include <iostream>
#include <fstream>
#include <iomanip>

using namespace std;

//opening a session
AttendanceSession& AttendanceRegister::openSession(string courseId, TimeSlot slot, int durationMin) {
    string sessId = courseId + "_SESS_" + to_string(sessions.size() + 1);
    AttendanceSession newSession(sessId, courseId, slot, "2026-09-17 10:00", durationMin);
    sessions.push_back(newSession);
    cout << "Successfully opened attendance session: " << sessId << " for course " << courseId << " (duration: " << durationMin << " mins)\n";
    return sessions.back();
}

AttendanceSession* AttendanceRegister::getActiveSession(string courseId) {
    for (int i = static_cast<int>(sessions.size()) - 1; i >= 0; i--) {
        if (sessions[i].getCourseId() == courseId && !sessions[i].isExpired()) {
            return &sessions[i];
        }
    }
    return nullptr;
}

AttendanceSession* AttendanceRegister::getActiveSession() {
    for (int i = static_cast<int>(sessions.size()) - 1; i >= 0; i--) {
        if (!sessions[i].isExpired()) {
            return &sessions[i];
        }
    }
    return nullptr;
}

AttendanceSession* AttendanceRegister::findSessionById(string sessionId) {
    for (size_t i = 0; i < sessions.size(); i++) {
        if (sessions[i].getId() == sessionId) {
            return &sessions[i];
        }
    }
    return nullptr;
}

void AttendanceRegister::processEvent(CaptureEvent event) {
    AttendanceSession* session = getActiveSession();
    if (!session || session->isExpired()) {
        throw SessionClosedException();
    }

    string studentId = event.getUid();
    // Check if already tapped
    for (AttendanceRecord* rec : session->getRecords()) {
        if (rec && rec->getStudentId() == studentId) {
            throw DuplicateAttendanceException();
        }
    }

    session->markPresent(studentId, "CardTap");
}

void AttendanceRegister::processEvent(CaptureEvent event,
                                      const std::function<std::string(const std::string&)>& uidToStudentId,
                                      const std::function<bool(const std::string&, const std::string&)>& isStudentEnrolled) {
    AttendanceSession* session = getActiveSession();
    if (!session || session->isExpired()) {
        throw SessionClosedException();
    }

    string rawUid = event.getUid();
    string studentId = uidToStudentId ? uidToStudentId(rawUid) : rawUid;

    if (isStudentEnrolled && !isStudentEnrolled(studentId, session->getCourseId())) {
        throw NotEnrolledException();
    }

    for (AttendanceRecord* rec : session->getRecords()) {
        if (rec && rec->getStudentId() == studentId) {
            throw DuplicateAttendanceException();
        }
    }

    session->markPresent(studentId, "ConsoleTap");
}

double AttendanceRegister::attendancePercent(string studentId, string courseId) const {
    int total = 0;
    int attended = 0;

    for (const AttendanceSession& sess : sessions) {
        if (sess.getCourseId() == courseId) {
            total++;
            for (AttendanceRecord* rec : sess.getRecords()) {
                if (rec && rec->getStudentId() == studentId) {
                    attended++;
                    break;
                }
            }
        }
    }

    if (total == 0) return 100.0;
    return (static_cast<double>(attended) / total) * 100.0;
}

void AttendanceRegister::courseAttendanceReport(string courseId) const {
    int totalSessions = 0;
    for (const AttendanceSession& sess : sessions) {
        if (sess.getCourseId() == courseId) totalSessions++;
    }

    cout << "\n========================================================\n";
    cout << "         ATTENDANCE REPORT FOR COURSE: " << courseId << "\n";
    cout << "========================================================\n";
    cout << " Total Sessions Held: " << totalSessions << "\n";
    if (totalSessions == 0) {
        cout << " No attendance sessions recorded for this course yet.\n";
        cout << "========================================================\n\n";
        return;
    }

    // List recent sessions
    cout << "\n Sessions breakdown:\n";
    for (const AttendanceSession& sess : sessions) {
        if (sess.getCourseId() == courseId) {
            cout << "  - Session: " << sess.getId() << " (Opened: " << sess.getOpenedAt()
                 << ", Status: " << (sess.getIsOpen() ? "OPEN" : "CLOSED") << ")\n";
            cout << "    Attendees (" << sess.getRecords().size() << "):\n";
            for (AttendanceRecord* rec : sess.getRecords()) {
                cout << "      * " << *rec << "\n";
            }
        }
    }
    cout << "========================================================\n\n";
}

void AttendanceRegister::courseAttendanceReport(string courseId, const vector<string>& enrolledStudents) const {
    int totalSessions = 0;
    for (const AttendanceSession& sess : sessions) {
        if (sess.getCourseId() == courseId) totalSessions++;
    }

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
        cout << left << setw(15) << stuId << setw(18) << (to_string(static_cast<int>(pct)) + "%");
        if (pct < 80.0 && totalSessions > 0) {
            cout << " [!] Ineligible (<80% threshold)\n";
        } else {
            cout << " [OK] Eligible\n";
        }
    }

    if (!enrolledStudents.empty()) {
        double avg = totalPct / enrolledStudents.size();
        cout << "--------------------------------------------------------\n";
        cout << " Overall Course Attendance Average: " << fixed << setprecision(1) << avg << "%\n";
    }
    cout << "========================================================\n\n";
}

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
        return; // file doesn't exist yet, nothing to load
    }

    sessions.clear();
    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;
        if (line.rfind("SESSION,", 0) == 0) {
            string sessData = line.substr(8);
            AttendanceSession* sess = AttendanceSession::fromLine(sessData);
            if (sess) {
                sessions.push_back(*sess);
                delete sess;
            }
        } else if (line.rfind("RECORD,", 0) == 0 || line.rfind("CORRECTION,", 0) == 0) {
            AttendanceRecord* rec = AttendanceRecord::fromLine(line);
            if (rec && !sessions.empty()) {
                // Find matching session or attach to last session
                AttendanceSession* target = findSessionById(rec->getSessionId());
                if (!target) target = &sessions.back();

                if (CorrectionRecord* cr = dynamic_cast<CorrectionRecord*>(rec)) {
                    target->appendCorrection(cr->getStudentId(), cr->getActingLecturerId(), cr->getReason(), false);
                    delete cr;
                } else {
                    target->markPresent(rec->getStudentId(), rec->getCapturedBy(), false);
                    delete rec;
                }
            } else if (rec) {
                delete rec;
            }
        }
    }
    file.close();
}