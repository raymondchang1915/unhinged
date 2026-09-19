#include "AttendanceSession.h"

#include <iostream>
#include <sstream>
using namespace std;

// a session stays open for 10 minutes unless the caller says otherwise
int AttendanceSession::defaultDurationMin = 10;

AttendanceSession::AttendanceSession(string sessId, string cId, TimeSlot s, string timeOpened, int duration)
    : id(sessId), courseId(cId), slot(s), openedAt(timeOpened), durationMin(duration), isOpen(true) {
}

// rule of 5: records are owned pointers, so every copy needs its own clones
AttendanceSession::AttendanceSession(const AttendanceSession& other)
    : id(other.id), courseId(other.courseId), slot(other.slot), openedAt(other.openedAt),
      durationMin(other.durationMin), isOpen(other.isOpen) {
    for (AttendanceRecord* rec : other.records) {
        if (rec) {
            records.push_back(rec->clone());
        }
    }
}

AttendanceSession& AttendanceSession::operator=(const AttendanceSession& other) {
    if (this != &other) {
        for (AttendanceRecord* rec : records) {
            delete rec;
        }
        records.clear();

        id = other.id;
        courseId = other.courseId;
        slot = other.slot;
        openedAt = other.openedAt;
        durationMin = other.durationMin;
        isOpen = other.isOpen;

        for (AttendanceRecord* rec : other.records) {
            if (rec) {
                records.push_back(rec->clone());
            }
        }
    }
    return *this;
}

// move: steal the records instead of cloning them, then leave the other side empty
AttendanceSession::AttendanceSession(AttendanceSession&& other) noexcept
    : id(move(other.id)), courseId(move(other.courseId)), slot(other.slot),
      openedAt(move(other.openedAt)), durationMin(other.durationMin), isOpen(other.isOpen),
      records(move(other.records)) {
    other.records.clear();
}

AttendanceSession& AttendanceSession::operator=(AttendanceSession&& other) noexcept {
    if (this != &other) {
        for (AttendanceRecord* rec : records) {
            delete rec;
        }
        records.clear();

        id = move(other.id);
        courseId = move(other.courseId);
        slot = other.slot;
        openedAt = move(other.openedAt);
        durationMin = other.durationMin;
        isOpen = other.isOpen;
        records = move(other.records);
        other.records.clear();
    }
    return *this;
}

AttendanceSession::~AttendanceSession() {
    for (AttendanceRecord* rec : records) {
        delete rec;
    }
    records.clear();
}

string AttendanceSession::getId() const {
    return id;
}

string AttendanceSession::getCourseId() const {
    return courseId;
}

TimeSlot AttendanceSession::getSlot() const {
    return slot;
}

string AttendanceSession::getOpenedAt() const {
    return openedAt;
}

int AttendanceSession::getDurationMin() const {
    return durationMin;
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

// a closed session counts as expired; taps are refused either way
bool AttendanceSession::isExpired() const {
    return !isOpen;
}

void AttendanceSession::markPresent(string studentId, string capturedBy, bool notify) {
    records.push_back(new AttendanceRecord(studentId, id, "2026-09-17 10:00", "Present", capturedBy));
    if (notify) {
        cout << "Recorded: Student " << studentId << " marked present." << endl;
    }
}

void AttendanceSession::appendCorrection(string studentId, string lecturerId, string reason, bool notify) {
    records.push_back(new CorrectionRecord(studentId, id, "2026-09-17 10:05", "Present", "Manual", lecturerId, reason));
    if (notify) {
        cout << "Correction appended for Student " << studentId << " by Lecturer " << lecturerId << endl;
    }
}

// id,courseId,day;start;end;location,openedAt,durationMin,isOpen
string AttendanceSession::toLine() const {
    return id + "," + courseId + "," + slot.toLine() + "," + openedAt + ","
         + to_string(durationMin) + "," + (isOpen ? "1" : "0");
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

    int duration = (durStr == "") ? defaultDurationMin : stoi(durStr);
    AttendanceSession* session = new AttendanceSession(sessId, cId, TimeSlot::fromLine(slotStr), timeOpened, duration);
    if (openStr == "0") {
        session->close();
    }
    return session;
}
