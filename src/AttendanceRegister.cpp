#include "AttendanceRegister.h"
#include "UniversitySystem.h"   // in the .cpp only, so the headers do not include each other
#include <fstream>
#include <iomanip>
#include <iostream>
using namespace std;

AttendanceRegister::AttendanceRegister() {
}
//sessions is a vector full of attendance sessions. defined in header
AttendanceSession& AttendanceRegister::openSession(string courseId, TimeSlot slot, int durationMin) {
    string sessId = courseId + "_SESS_" + to_string(sessions.size() + 1); //the course ID plus the session.size creates a unique session ID
    AttendanceSession newSession(sessId, courseId, slot, "2026-09-17 10:00", durationMin); //this is intialisation of the new ID
    sessions.push_back(newSession); //pushing the new session back to the stack

    cout << "Successfully opened attendance session: " << sessId
         << " for course " << courseId << " (duration: " << durationMin << " mins)\n";
    return sessions.back();
}
// since sessions.size return an unsigned integer (int) is used to turn it back to int
// search in reverse since we are working on a stack
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
        if (!sessions[i].isExpired()) {//not expired
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

vector<AttendanceSession>& AttendanceRegister::getAllSessions() {
    return sessions;
}

const vector<AttendanceSession>& AttendanceRegister::getAllSessions() const {
    return sessions;
}

// how a card tap is processed
void AttendanceRegister::processEvent(CaptureEvent event) {
    AttendanceSession* session = getActiveSession();
    if (session == nullptr) {   // check if there are any sessions active
        throw SessionClosedException();
    }

    UniversitySystem& sys = UniversitySystem::getInstance();  //create an instance of the app

    // the card carries a UID like CARD_S001, so turn it into a student ID
    string studentId = sys.resolveUidToStudentId(event.getUid());

    // the student must be enrolled in the course this session belongs to
    bool enrolled = false; //initialised to false
    vector<Enrolment*> theirEnrolments = sys.getEnrolmentsForStudent(studentId);// the universitysystem class object sys is used to get the enrollment data
    for (int i = 0; i < (int)theirEnrolments.size(); i++) {  //from all their enrollemnt data we check if this course is available
        if (theirEnrolments[i]->getCourseId() == session->getCourseId()) {
            enrolled = true;
        }
    }
    if (!enrolled) {
        throw NotEnrolledException();
    }

    // check if they have already tapped by comparing the exitingg record
    const vector<AttendanceRecord*>& records = session->getRecords();
    for (int i = 0; i < (int)records.size(); i++) {
        if (records[i] != nullptr && records[i]->getStudentId() == studentId) {
            throw DuplicateAttendanceException();
        }
    }

    session->markPresent(studentId, "CardTap");
}
//count number of sessions per course
int AttendanceRegister::countSessionsFor(const string& courseId) const {
    int total = 0;
    for (int i = 0; i < (int)sessions.size(); i++) {
        if (sessions[i].getCourseId() == courseId) {
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
    for (int i = 0; i < (int)sessions.size(); i++) {
        if (sessions[i].getCourseId() != courseId) {
            continue; //skip the loop entirely
        }
        const vector<AttendanceRecord*>& records = sessions[i].getRecords();
        for (int j = 0; j < (int)records.size(); j++) {
            if (records[j] != nullptr && records[j]->getStudentId() == studentId) {
                attended++;
                break; //if not null and matches student id add one to attendance andstop the loop.
            }
        }
    }

    return ((double)attended / total) * 100.0;
}

void AttendanceRegister::courseAttendanceReport(string courseId) const {
    int totalSessions = countSessionsFor(courseId);

    cout << "         ATTENDANCE REPORT FOR COURSE: " << courseId << "\n";
    cout << " Total Sessions Held: " << totalSessions << "\n";

    if (totalSessions == 0) {
        cout << " No attendance sessions recorded for this course yet.\n";
        return;
    }

    cout << "\n Sessions breakdown:\n";
    for (int i = 0; i < (int)sessions.size(); i++) {
        if (sessions[i].getCourseId() != courseId) {
            continue; //skip this step
        }

        string state = "CLOSED";
        if (sessions[i].getIsOpen()) {
            state = "OPEN";
        }

        const vector<AttendanceRecord*>& records = sessions[i].getRecords();
        cout << "  - Session: " << sessions[i].getId()
             << " (Opened: " << sessions[i].getOpenedAt() << ", Status: " << state << ")\n";
        cout << "    Attendees (" << records.size() << "):\n";

        for (int j = 0; j < (int)records.size(); j++) {
            if (records[j] != nullptr) {
                cout << "      * " << *records[j] << "\n";
            }
        }
    }

    // part 2: every enrolled student, including the ones who never turned up.
    // the roll comes from Course, because the register only knows who tapped.
    Course* course = UniversitySystem::getInstance().findCourse(courseId);
    if (course == nullptr) {
        cout << "========================================================\n\n";
        return;
    }
    vector<string> enrolledStudents = course->getEnrolledIds();

    cout << "\n Student summary:\n";
    cout << " Enrolled Students:   " << enrolledStudents.size() << "\n";
    cout << "--------------------------------------------------------\n";
    cout << left << setw(15) << "Student ID" << setw(20) << "Attendance %" << "Status\n";
    cout << "--------------------------------------------------------\n";

    double totalPct = 0;
    for (int i = 0; i < (int)enrolledStudents.size(); i++) {
        double pct = attendancePercent(enrolledStudents[i], courseId);
        totalPct = totalPct + pct;

        cout << left << setw(15) << enrolledStudents[i] << setw(18) << (to_string((int)pct) + "%");
        if (pct < 80.0 && totalSessions > 0) {
            cout << " [!] Ineligible (<80% threshold)\n";
        } else {
            cout << " [OK] Eligible\n";
        }
    }

    if (enrolledStudents.size() > 0) {
        double average = totalPct / enrolledStudents.size();
        cout << "--------------------------------------------------------\n";
        cout << " Overall Course Attendance Average: " << fixed << setprecision(1) << average << "%\n";
    }
    cout << "========================================================\n\n";
}

// attendance.txt holds a SESSION line followed by that session's record lines
void AttendanceRegister::save(const string& path) const {
    ofstream file(path);
    if (!file.is_open()) {
        throw PersistenceException("Error: Could not save attendance file: " + path);
    }

    for (int i = 0; i < (int)sessions.size(); i++) {
        file << "SESSION," << sessions[i].toLine() << "\n";

        const vector<AttendanceRecord*>& records = sessions[i].getRecords(); //using the saving template
        for (int j = 0; j < (int)records.size(); j++) {
            if (records[j] != nullptr) {
                file << records[j]->toLine() << "\n";
            }
        }
    }
    file.close();
}

//check if there's anythign to load
//check if it's a new session that has to be loaded if so create new session
//check if it's a data like aa record or a correction record if so
//use dynamic cast to differ between normal and correction record loading
//if correction create aa correction record else markpresnt


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

        // a SESSION line starts a new session
        if (line.substr(0, 8) == "SESSION,") { //if the substring is SESSION create a new object
            AttendanceSession* session = AttendanceSession::fromLine(line.substr(8));
            if (session != nullptr) {
                sessions.push_back(*session);
                delete session; //push to the stack and delete the pointer
            }
            continue;
        }

        bool isRecord = (line.substr(0, 7) == "RECORD," || line.substr(0, 11) == "CORRECTION,");
        if (!isRecord || sessions.size() == 0) {
            continue;
        }

        AttendanceRecord* record = AttendanceRecord::fromLine(line);
        if (record == nullptr) {
            continue;
        }

        // put the record back on its own session, or the newest one if the ID is unknown
        AttendanceSession* target = findSessionById(record->getSessionId());
        if (target == nullptr) {
            target = &sessions.back();
        }

        // notify = false, because this is a reload and not a live tap
        CorrectionRecord* correction = dynamic_cast<CorrectionRecord*>(record);
        if (correction != nullptr) {
            target->appendCorrection(correction->getStudentId(), correction->getActingLecturerId(),
                                     correction->getReason(), false);
        } else {
            target->markPresent(record->getStudentId(), record->getCapturedBy(), false);
        }
        delete record;
    }
    file.close();
}
