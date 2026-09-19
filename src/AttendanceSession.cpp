#include "AttendanceSession.h"
#include <iostream>
#include <sstream>
using namespace std;

//static default duration to 10 mins
int AttendanceSession::defaultDurationMin=10;

//constructor
AttendanceSession::AttendanceSession(string sessId,string cId, TimeSlot s, string timeOpened, int duration):slot(s)
{
    //for internal variables this->
    this->id=sessId;
    this->courseId=cId;
    this->openedAt = timeOpened;
    this->durationMin=duration;
    this->isOpen = true; //a session is open when it is created
}

//deep copy constructer / rule of 5
AttendanceSession::AttendanceSession(const AttendanceSession& other)
    : id(other.id),courseId(other.courseId), slot(other.slot),
        openedAt(other.openedAt), durationMin(other.durationMin),isOpen(other.isOpen){
            for (AttendanceRecord* rec : other.records){
                if (rec) records.push_back(rec->clone());
    }
}

//deep copy assignment operator
AttendanceSession& AttendanceSession::operator=(const AttendanceSession& other){
    if(this != &other){
        for (AttendanceRecord* rec: records)delete rec;
        records.clear();

        id = other.id;
        courseId = other.courseId;
        slot = other.slot;
        openedAt = other.openedAt;
        durationMin = other.durationMin;
        isOpen = other.isOpen;
        for(AttendanceRecord* rec : other.records){
            if(rec) records.push_back(rec->clone());
        }
    }return *this;
}

//move constructor
AttendanceSession& AttendanceSession::operator=(AttendanceSession&& other)noexcept{
    if (this!= &other){
        for (AttendanceRecord* rec:records)delete rec;
        records.clear();

        id = std::move(other.id);
        courseId = std::move(other.courseId);
        slot = other.slot;
        openedAt = std::move(other.openedAt);
        durationMin = other.durationMin;
        isOpen = other.isOpen;
        records = std::move(other.records);
        other.records.clear();
    }
    return *this;
}

string AttendanceSession::getId() const { return id; }
string AttendanceSession::getCourseId() const { return courseId; }
TimeSlot AttendanceSession::getSlot() const { return slot; }
string AttendanceSession::getOpenedAt() const { return openedAt; }
int AttendanceSession::getDurationMin() const { return durationMin; }
bool AttendanceSession::getIsOpen() const { return isOpen; }
const vector<AttendanceRecord*>& AttendanceSession::getRecords() const { return records; }

void AttendanceSession::close(){
    isOpen=false;
    cout<<"Session "<<id<<" is now closed."<<endl;
}

bool AttendanceSession::isExpired() const {
    if (!isOpen) {
        return true;
    }
    return false;
}

void AttendanceSession::markPresent(string studentId,string capturedBy, bool notify) {
    AttendanceRecord* newRecord = new AttendanceRecord(studentId, id, "2026-09-17 10:00", "Present", capturedBy);
    records.push_back(newRecord);
    if (notify) {
        cout<<"Recorded: Student "<<studentId<<" marked present."<<endl;
    }
}

void AttendanceSession::appendCorrection(string studentId,string lecturerId,string reason, bool notify) {
    AttendanceRecord* correction = new CorrectionRecord(studentId,id,"2026-09-17 10:05","Present","Manual",lecturerId,reason);
    records.push_back(correction);
    if (notify) {
        cout<<"Correction appended for Student "<<studentId<<" by Lecturer "<< lecturerId <<endl;
    }
}

//destructor
AttendanceSession::~AttendanceSession()
{
    for (size_t i=0;i<records.size();i++){
        delete records[i];
    }
    records.clear();
}

string AttendanceSession::toLine() const{
    return id + "," + courseId + "," + slot.toLine() + "," + openedAt + "," + to_string(durationMin) + "," + (isOpen ? "1" : "0");
}

AttendanceSession* AttendanceSession::fromLine(string line) {
    //string stream parsing logic
    stringstream ss(line);
    string sessId, cId, slotStr, timeOpened, durStr, openStr;
    getline(ss, sessId, ',');
    getline(ss, cId, ',');
    getline(ss, slotStr, ',');
    getline(ss, timeOpened, ',');
    getline(ss, durStr, ',');
    getline(ss, openStr);

    if (sessId.empty() || cId.empty()) return nullptr;
    int duration = durStr.empty() ? 10 : stoi(durStr);
    TimeSlot s = TimeSlot::fromLine(slotStr);
    AttendanceSession* session = new AttendanceSession(sessId, cId, s, timeOpened, duration);
    if (openStr == "0") session->close();
    return session;
}

