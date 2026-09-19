#ifndef CORRECTIONRECORD_H
#define CORRECTIONRECORD_H
#include "AttendanceRecord.h"
#include <string>

class CorrectionRecord:public AttendanceRecord{
    private:
    std::string actingLecturerId;
    std::string reason;

    public:
    CorrectionRecord(std::string stuId,std::string sessId,std::string ts,std::string stat,std::string capBy,std::string lecturerId,std::string rsn);
    std::string getActingLecturerId() const;
    std::string getReason() const;
    AttendanceRecord* clone() const override;
    std::string toLine() const override;

};

#endif