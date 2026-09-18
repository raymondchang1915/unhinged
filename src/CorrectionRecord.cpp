#include "CorrectionRecord.h"
#include <string>

//constructor
CorrectionRecord::CorrectionRecord(std::string stuId,std::string sessId,std::string ts,std::string stat,std::string capBy,std::string lecturerId,std::string rsn)
    :AttendanceRecord(stuId, sessId, ts, stat, capBy) {

    this->actingLecturerId = lecturerId;
    this->reason = rsn;
}

std::string CorrectionRecord::getActingLecturerId() const {
    return actingLecturerId;
}

std::string CorrectionRecord::getReason() const {
    return reason;
}

AttendanceRecord* CorrectionRecord::clone() const {
    return new CorrectionRecord(*this);
}

std::string CorrectionRecord::toLine() const {
    return "CORRECTION," + studentId + "," + sessionId + "," + timestamp + "," + status + "," + capturedBy + "," + actingLecturerId + "," + reason;
}