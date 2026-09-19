#include "AttendanceRecord.h"
#include "CorrectionRecord.h"

#include <sstream>
using namespace std;

AttendanceRecord::AttendanceRecord(string stuId, string sessId, string ts, string stat, string capBy)
    : studentId(stuId), sessionId(sessId), timestamp(ts), status(stat), capturedBy(capBy) {
}

string AttendanceRecord::getStudentId() const {
    return studentId;
}

string AttendanceRecord::getSessionId() const {
    return sessionId;
}

string AttendanceRecord::getCapturedBy() const {
    return capturedBy;
}

AttendanceRecord* AttendanceRecord::clone() const {
    return new AttendanceRecord(*this);
}

// RECORD,studentId,sessionId,timestamp,status,capturedBy
string AttendanceRecord::toLine() const {
    return "RECORD," + studentId + "," + sessionId + "," + timestamp + "," + status + "," + capturedBy;
}

// reads the tag first, then builds the matching class - same pattern as Person and Course
AttendanceRecord* AttendanceRecord::fromLine(string line) {
    stringstream ss(line);
    string tag, stuId, sessId, ts, stat, capBy, lecturerId, reason;

    getline(ss, tag, ',');
    getline(ss, stuId, ',');
    getline(ss, sessId, ',');
    getline(ss, ts, ',');
    getline(ss, stat, ',');
    getline(ss, capBy, ',');

    if (stuId == "" || sessId == "") {
        return nullptr;
    }

    if (tag == "CORRECTION") {
        getline(ss, lecturerId, ',');
        getline(ss, reason);
        return new CorrectionRecord(stuId, sessId, ts, stat, capBy, lecturerId, reason);
    }

    return new AttendanceRecord(stuId, sessId, ts, stat, capBy);
}

// friend, so it can read the protected fields directly
ostream& operator<<(ostream& os, const AttendanceRecord& r) {
    os << r.studentId << " - " << r.status << " at " << r.timestamp
       << " (captured by " << r.capturedBy << ")";
    return os;
}
