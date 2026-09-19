#ifndef CAPTUREEVENT_H
#define CAPTUREEVENT_H
#include <string>
class CaptureEvent{
private:
    std::string uid;
    std::string timestamp;
public:
    CaptureEvent(std::string u,std::string t)
    {
        this->uid=u;
        this->timestamp=t;
    }
    std::string getUid() const {return uid;}
    std::string getTimestamp() const {return timestamp;}
};

#endif