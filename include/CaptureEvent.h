#ifndef CAPTUREEVENT_H
#define CAPTUREEVENT_H

#include <string>
using namespace std;

// one card tap: who tapped and when
class CaptureEvent {
private:
    string uid;
    string timestamp;

public:
    CaptureEvent(string u, string t) : uid(u), timestamp(t) {
    }

    string getUid() const {
        return uid;
    }

    string getTimestamp() const {
        return timestamp;
    }
};

#endif
