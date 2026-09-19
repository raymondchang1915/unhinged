#ifndef ATTENDANCECAPTURE_H
#define ATTENDANCECAPTURE_H

#include "CaptureEvent.h"

// abstract interface: the register takes taps from any of these without
// knowing whether a person typed them or a file replayed them
class AttendanceCapture {
public:
    virtual void beginSession() = 0;

    virtual CaptureEvent captureNext() = 0;   // returns the UID and timestamp of one tap

    virtual void endSession() = 0;

    virtual ~AttendanceCapture() = default;   // so deleting through the base runs the child's destructor
};

#endif
