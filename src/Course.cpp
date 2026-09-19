#include "Course.h"
#include "LectureCourse.h"
#include "LabCourse.h"
#include "ProjectCourse.h"
#include "Exceptions.h"

#include <sstream>

Course::Course(string c, string t, int cv, int cap, string lect)
    : code(c), title(t), creditVal(cv), capacity(cap), assignedLectId(lect) {
}

Course::~Course() {
}

string Course::getId() const {
    return code;
}

string Course::getTitle() const {
    return title;
}

int Course::getCreditVal() const {
    return creditVal;
}

int Course::getCapacity() const {
    return capacity;
}

string Course::getAssignedLectId() const {
    return assignedLectId;
}

vector<string> Course::getPrerequisites() const {
    return prereq;
}

vector<string> Course::getEnrolledIds() const {
    return enrolledIds;
}

const vector<TimeSlot>& Course::getSlots() const {
    return slots;
}

bool Course::isFull() const {
    return (int)enrolledIds.size() >= capacity;
}

void Course::addStudent(string studentId) {
    enrolledIds.push_back(studentId);
}

void Course::removeStudent(string studentId) {
    for (int i = 0; i < (int)enrolledIds.size(); i++) {
        if (enrolledIds[i] == studentId) {
            enrolledIds.erase(enrolledIds.begin() + i);
            return;
        }
    }
}

void Course::addPrereq(string courseCode) {
    prereq.push_back(courseCode);
}

void Course::addSlot(const TimeSlot& slot) {
    slots.push_back(slot);
}

// prerequisites are ';' separated, NONE when there are none
string Course::prereqToLine() const {
    if (prereq.empty()) {
        return "NONE";
    }
    string out = prereq[0];
    for (int i = 1; i < (int)prereq.size(); i++) {
        out += ";" + prereq[i];
    }
    return out;
}

// slots are '|' separated; each slot is day;start;end;location (TimeSlot::toLine)
string Course::slotsToLine() const {
    string out = "";
    for (int i = 0; i < (int)slots.size(); i++) {
        if (i > 0) {
            out += "|";
        }
        out += slots[i].toLine();
    }
    return out;
}

// the shared middle of a courses.txt line; each subclass wraps it with its tag and extra field
string Course::toLine() const {
    return code + "," + title + "," + to_string(creditVal) + ","
         + to_string(capacity) + "," + assignedLectId;
}

// courses.txt format:  TAG,code,title,credits,capacity,lecturerId,extra,prereq,slots
// TAG is LEC, LAB or PROJ. extra is labHours for LAB, 1/0 pass-fail for PROJ, 0 for LEC.
Course* Course::fromLine(string line) {
    stringstream ss(line);
    string tag, code, title, creditStr, capStr, lectId, extra, prereqStr, slotStr;

    getline(ss, tag, ',');
    getline(ss, code, ',');
    getline(ss, title, ',');
    getline(ss, creditStr, ',');
    getline(ss, capStr, ',');
    getline(ss, lectId, ',');
    getline(ss, extra, ',');
    getline(ss, prereqStr, ',');
    getline(ss, slotStr);

    if (code == "" || creditStr == "" || capStr == "") {
        throw CorruptDataException("courses.txt", 0);
    }

    int credits = stoi(creditStr);
    int capacity = stoi(capStr);

    Course* c = nullptr;
    if (tag == "LEC") {
        c = new LectureCourse(code, title, credits, capacity, lectId);
    } else if (tag == "LAB") {
        int labHours = (extra == "") ? 0 : stoi(extra);
        c = new LabCourse(code, title, credits, capacity, lectId, labHours);
    } else if (tag == "PROJ") {
        c = new ProjectCourse(code, title, credits, capacity, lectId, extra == "1");
    } else {
        throw CorruptDataException("courses.txt", 0);
    }

    if (prereqStr != "" && prereqStr != "NONE") {
        stringstream ps(prereqStr);
        string one;
        while (getline(ps, one, ';')) {
            if (one != "") {
                c->addPrereq(one);
            }
        }
    }

    if (slotStr != "" && slotStr != "NONE") {
        stringstream sls(slotStr);
        string one;
        while (getline(sls, one, '|')) {
            if (one != "") {
                c->addSlot(TimeSlot::fromLine(one));
            }
        }
    }

    return c;
}

ostream& operator<<(ostream& os, const Course& c) {
    os << c.getId() << " - " << c.getTitle()
       << " (" << c.calculateCredits() << " credits)";
    return os;
}
