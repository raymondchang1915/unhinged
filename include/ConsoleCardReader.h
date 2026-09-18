#ifndef CONSOLECARDREADER_H
#define CONSOLECARDREADER_H

#include "AttendanceCapture.h"
#include <string>

class ConsoleCardReader: public AttendanceCapture{
    public:
    void beginSession() override;

    CaptureEvent captureNext() override;

    void endSession() override;
};

#endif
