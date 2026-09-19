#ifndef CORRECTIONRECORD_H
#define CORRECTIONRECORD_H

#include "AttendanceRecord.h"

#include <string>
using namespace std;

// a lecturer's manual fix, stored beside the original record instead of replacing it
class CorrectionRecord : public AttendanceRecord {
private:
    string actingLecturerId;
    string reason;

public:
    CorrectionRecord(string stuId, string sessId, string ts, string stat, string capBy,
                     string lecturerId, string rsn);

    string getActingLecturerId() const;
    string getReason() const;

    AttendanceRecord* clone() const override;
    string toLine() const override;
};

#endif
