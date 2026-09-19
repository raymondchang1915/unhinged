#include "AttendanceSession.h"

#include <iostream>
#include <sstream>
using namespace std;

// a session stays open for 10 minutes unless the caller says otherwise
int AttendanceSession::defaultDurationMin = 10;

AttendanceSession::AttendanceSession(string sessId, string cId, TimeSlot s, string timeOpened, int duration)
    : id(sessId), courseId(cId), slot(s), openedAt(timeOpened), durationMin(duration), isOpen(true) {
}

// copy constructor: deep copy this is needed in the case of push back to vectors and when vectors resize. this creates a new object 
AttendanceSession::AttendanceSession(const AttendanceSession& other)
    : id(other.id), courseId(other.courseId), slot(other.slot), openedAt(other.openedAt),
      durationMin(other.durationMin), isOpen(other.isOpen) {
    for (int i = 0; i < (int)other.records.size(); i++) {
        if (other.records[i] != nullptr) {
            records.push_back(other.records[i]->clone());
        }
    }
}
//overwrite an existing object with a deep copy of another
AttendanceSession& AttendanceSession::operator=(const AttendanceSession& other) {
    if (this != &other) {
        for (int i = 0; i < (int)records.size(); i++) {
            delete records[i];
        }
        records.clear();

        id = other.id;
        courseId = other.courseId;
        slot = other.slot;
        openedAt = other.openedAt;
        durationMin = other.durationMin;
        isOpen = other.isOpen;

        for (int i = 0; i < (int)other.records.size(); i++) {
            if (other.records[i] != nullptr) {
                records.push_back(other.records[i]->clone());
            }
        }
    }
    return *this;
}

// move: builds a brand new but instead move the daaata rather than cloning it
AttendanceSession::AttendanceSession(AttendanceSession&& other) noexcept
    : id(other.id), courseId(other.courseId), slot(other.slot), openedAt(other.openedAt),
      durationMin(other.durationMin), isOpen(other.isOpen), records(other.records) {
    other.records.clear();
}
//move assignment operator: moving the stolen data frm a temporaary object to an exisitng one
AttendanceSession& AttendanceSession::operator=(AttendanceSession&& other) noexcept {
    if (this != &other) {
        for (int i = 0; i < (int)records.size(); i++) {
            delete records[i];
        }
        records.clear();

        id = other.id;
        courseId = other.courseId;
        slot = other.slot;
        openedAt = other.openedAt;
        durationMin = other.durationMin;
        isOpen = other.isOpen;
        records = other.records;
        other.records.clear();
    }
    return *this;
}

AttendanceSession::~AttendanceSession() {
    for (int i = 0; i < (int)records.size(); i++) {
        delete records[i];
    }
    records.clear();
}

string AttendanceSession::getId() const {
    return id;
}

string AttendanceSession::getCourseId() const {
    return courseId;
}

string AttendanceSession::getOpenedAt() const {
    return openedAt;
}

bool AttendanceSession::getIsOpen() const {
    return isOpen;
}

const vector<AttendanceRecord*>& AttendanceSession::getRecords() const {
    return records;
}

void AttendanceSession::close() {
    isOpen = false;
    cout << "Session " << id << " is now closed." << endl;
}

// a closed session counts as expired, and taps are refused either way
bool AttendanceSession::isExpired() const {
    return !isOpen;
}

void AttendanceSession::markPresent(string studentId, string capturedBy, bool notify) {
    AttendanceRecord* record = new AttendanceRecord(studentId, id, "2026-09-17 10:00", "Present", capturedBy);
    records.push_back(record);
    if (notify) {
        cout << "Recorded: Student " << studentId << " marked present." << endl;
    }
}

void AttendanceSession::appendCorrection(string studentId, string lecturerId, string reason, bool notify) {
    AttendanceRecord* correction = new CorrectionRecord(studentId, id, "2026-09-17 10:05",
                                                        "Present", "Manual", lecturerId, reason);
    records.push_back(correction);
    if (notify) {
        cout << "Correction appended for Student " << studentId << " by Lecturer " << lecturerId << endl;
    }
}

// id,courseId,day;start;end;location,openedAt,durationMin,isOpen
string AttendanceSession::toLine() const {
    string openFlag = "0";
    if (isOpen) {
        openFlag = "1";
    }
    return id + "," + courseId + "," + slot.toLine() + "," + openedAt + ","
         + to_string(durationMin) + "," + openFlag;
}

AttendanceSession* AttendanceSession::fromLine(string line) {
    stringstream ss(line);
    string sessId, cId, slotStr, timeOpened, durStr, openStr;

    getline(ss, sessId, ',');
    getline(ss, cId, ',');
    getline(ss, slotStr, ',');
    getline(ss, timeOpened, ',');
    getline(ss, durStr, ',');
    getline(ss, openStr);

    if (sessId == "" || cId == "") {
        return nullptr;
    }

    int duration = defaultDurationMin;
    if (durStr != "") {
        duration = stoi(durStr);
    }

    TimeSlot slot = TimeSlot::fromLine(slotStr);
    AttendanceSession* session = new AttendanceSession(sessId, cId, slot, timeOpened, duration);
    if (openStr == "0") {
        session->close();
    }
    return session;
}
