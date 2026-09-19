#ifndef FILEREPLAYCAPTURE_H
#define FILEREPLAYCAPTURE_H

#include "AttendanceCapture.h"
#include <string>
#include <fstream>

class FileReplayCapture:public AttendanceCapture {//filereplay capture inherits from attendancecapture
private:
    std::string filePath;
    std::ifstream fileStream;

public:
    FileReplayCapture(std::string path);

    void beginSession() override;//ignores parent classes generic function
    CaptureEvent captureNext() override;
    void endSession() override;
};

#endif