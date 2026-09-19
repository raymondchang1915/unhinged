#include "CorrectionRecord.h"
using namespace std;

CorrectionRecord::CorrectionRecord(string stuId, string sessId, string ts, string stat,
                                   string capBy, string lecturerId, string rsn)
    : AttendanceRecord(stuId, sessId, ts, stat, capBy), actingLecturerId(lecturerId), reason(rsn) {
}

string CorrectionRecord::getActingLecturerId() const {
    return actingLecturerId;
}

string CorrectionRecord::getReason() const {
    return reason;
}

AttendanceRecord* CorrectionRecord::clone() const {
    return new CorrectionRecord(*this);
}

// CORRECTION,...same six fields...,lecturerId,reason
string CorrectionRecord::toLine() const {
    return "CORRECTION," + studentId + "," + sessionId + "," + timestamp + "," + status + ","
         + capturedBy + "," + actingLecturerId + "," + reason;
}
