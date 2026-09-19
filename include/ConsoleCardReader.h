#ifndef CONSOLECARDREADER_H
#define CONSOLECARDREADER_H

#include "AttendanceCapture.h"

#include <string>
using namespace std;

// stands in for a real USB card reader: the UID is typed at the keyboard
class ConsoleCardReader : public AttendanceCapture {
public:
    void beginSession() override;
    CaptureEvent captureNext() override;
    void endSession() override;
};

#endif
