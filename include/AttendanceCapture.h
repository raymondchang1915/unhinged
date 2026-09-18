#ifndef ATTENDANCECAPTURE_H
#define ATTENDANCECAPTURE_H
#include "CaptureEvent.h"

//An abstract interface
class AttendanceCapture{
    public:
    virtual void beginSession()=0;

    //Returns UID & timestamp
    virtual CaptureEvent captureNext()=0;

    virtual void endSession()=0;

    //Destructor for child destructor then own
    virtual ~AttendanceCapture()=default;
};

#endif