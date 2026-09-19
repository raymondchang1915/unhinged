#include "AttendanceRecord.h"
#include "CorrectionRecord.h"

#include <sstream>

AttendanceRecord::AttendanceRecord(std::string stuId, std::string sessId, std::string ts,
                                   std::string stat, std::string capBy)
    : studentId(stuId), sessionId(sessId), timestamp(ts), status(stat), capturedBy(capBy) {
}

std::string AttendanceRecord::getStudentId() const {
    return studentId;
}

std::string AttendanceRecord::getSessionId() const {
    return sessionId;
}

std::string AttendanceRecord::getTimestamp() const {
    return timestamp;
}

std::string AttendanceRecord::getStatus() const {
    return status;
}

std::string AttendanceRecord::getCapturedBy() const {
    return capturedBy;
}

// virtual copy: a session copies its records without knowing which kind each one is
AttendanceRecord* AttendanceRecord::clone() const {
    return new AttendanceRecord(*this);
}

// RECORD,studentId,sessionId,timestamp,status,capturedBy
std::string AttendanceRecord::toLine() const {
    return "RECORD," + studentId + "," + sessionId + "," + timestamp + "," + status + "," + capturedBy;
}

// reads the tag first, then builds the matching subclass - same pattern as Person and Course
AttendanceRecord* AttendanceRecord::fromLine(std::string line) {
    std::stringstream ss(line);
    std::string tag, stuId, sessId, ts, stat, capBy, lecturerId, reason;

    std::getline(ss, tag, ',');
    std::getline(ss, stuId, ',');
    std::getline(ss, sessId, ',');
    std::getline(ss, ts, ',');
    std::getline(ss, stat, ',');
    std::getline(ss, capBy, ',');

    if (stuId == "" || sessId == "") {
        return nullptr;
    }

    if (tag == "CORRECTION") {
        std::getline(ss, lecturerId, ',');
        std::getline(ss, reason);
        return new CorrectionRecord(stuId, sessId, ts, stat, capBy, lecturerId, reason);
    }

    return new AttendanceRecord(stuId, sessId, ts, stat, capBy);
}

// friend, so it can read the protected fields directly
std::ostream& operator<<(std::ostream& os, const AttendanceRecord& r) {
    os << r.studentId << " - " << r.status << " at " << r.timestamp
       << " (captured by " << r.capturedBy << ")";
    return os;
}
