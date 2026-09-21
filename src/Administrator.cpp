#include "Administrator.h"
#include "UniversitySystem.h"
#include "Student.h"
#include "Lecturer.h"
#include "Course.h"
#include "LectureCourse.h"
#include "LabCourse.h"
#include "ProjectCourse.h"
#include "Enrolment.h"
#include "Exceptions.h"

#include <iostream>
#include <iomanip>

// Reads one whole number. If the user types something that is not a number,
// cin goes into a fail state and every later read is skipped, which used to
// save a course with empty fields. Clearing the stream and throwing stops that.
// A plain file level function, not a member, so no class gains a new method.
static int readNumber(const string& prompt) {
    cout << prompt;
    int value = 0;
    if (!(cin >> value)) {
        cin.clear();
        string junk;
        getline(cin, junk);
        throw SystemException("That field needs a whole number.");
    }
    return value;
}

Administrator::Administrator(string i, string h, string p) : Person(i, h, p) {
}

Administrator::~Administrator() {
}

void Administrator::showDashboard() {
    cout << "\n--- Administrator Dashboard ---\n";
    cout << "Logged in as: " << getName() << " (" << getId() << ")\n";
    cout << "1. Manage users\n";
    cout << "2. Manage courses\n";
    cout << "3. Enrolment report\n";
    cout << "4. Logout\n";
}

void Administrator::createUser() {
    UniversitySystem& sys = UniversitySystem::getInstance();

    string role, id, name, password;
//the data is taken here
    cout << "Role (Student / Lecturer / Admin): ";
    cin >> role;
    cout << "User ID: ";
    cin >> id;
    cout << "Full name: ";
    cin >> ws;              // drop the leftover newline
    getline(cin, name);     // getline used so the name can have spaces
    cout << "Password: ";
    cin >> password;

    if (sys.getUsers().findById(id) != nullptr) {
        throw SystemException("A user with ID " + id + " already exists.");
    }
    if (name.find(',') != string::npos) {
        throw SystemException("Name cannot contain a comma, it would break users.txt.");
    }
//from the data that was took the object is created here
    Person* user = nullptr;
    if (role == "Student") {
        user = new Student(id, name, password, "CARD_" + id);
    } else if (role == "Lecturer") {
        user = new Lecturer(id, name, password);
    } else if (role == "Admin") {
        user = new Administrator(id, name, password);
    } else {
        throw SystemException("Unknown role '" + role + "'. Use Student, Lecturer or Admin.");
    }

    sys.getUsers().add(user);
    sys.saveAll();
    cout << "[Success] Created " << role << " " << id << " (" << name << ")\n";
}

// Person has no setters, so an edit builds a new object of the same role and
// lets Repository::add replace the old one
void Administrator::updateUser(string userId) {
    UniversitySystem& sys = UniversitySystem::getInstance();

    Person* user = sys.getUsers().findById(userId); //check if user exist
    if (user == nullptr) {
        throw UserNotFoundException(userId);
    }
//get the new username and password
    string name, password;
    cout << "New full name: ";
    cin >> ws;
    getline(cin, name);
    cout << "New password: ";
    cin >> password;
//remove any commas
    if (name.find(',') != string::npos) {
        throw SystemException("Name cannot contain a commas");
    }
//check if the user is student or lecturer
    Person* updated = nullptr;
    Student* student = dynamic_cast<Student*>(user);
    Lecturer* lecturer = dynamic_cast<Lecturer*>(user);
//if student
    if (student != nullptr) {
        // the card and the completed courses have to carry across
        Student* newStudent = new Student(userId, name, password, student->getCard().getUid());
        //create new student objet and run a loop to get all the completed courses from the old object
        vector<string> completed = student->getCompletedCourses();
        for (int i = 0; i < (int)completed.size(); i++) {
            newStudent->addCompletedCourse(completed[i]);
        }
        updated = newStudent;
    } else if (lecturer != nullptr) {
        updated = new Lecturer(userId, name, password);
    } else {
        updated = new Administrator(userId, name, password);
    }

    sys.getUsers().add(userId, updated);   // deletes the old object, stores the new one using the repository technique of add
    sys.rebuildStudentTimetables();        // the replacement student starts with an empty timetable
    sys.saveAll();
    cout << "[Success] Updated user " << userId << "\n";
}

void Administrator::removeUser(string userId) {
    UniversitySystem& sys = UniversitySystem::getInstance();
    if (!sys.getUsers().findById(userId)) { //check if user exist
        throw UserNotFoundException(userId);
    }
    sys.getUsers().remove(userId); //use repo power
    sys.saveAll();
    cout << "[Success] Removed user " << userId << "\n";
}

void Administrator::createCourse() {
    UniversitySystem& sys = UniversitySystem::getInstance();

    string type, code, title, lectId;
    int credits = 0;
    int capacity = 0;

    cout << "Type (LEC / LAB / PROJ): ";
    cin >> type;
    cout << "Course code: ";
    cin >> code;
    cout << "Title: ";
    cin >> ws;
    getline(cin, title);
    credits = readNumber("Credits: ");
    capacity = readNumber("Capacity: ");
    cout << "Lecturer ID: ";
    cin >> lectId;

    if (sys.findCourse(code) != nullptr) {
        throw SystemException("A course with code " + code + " already exists.");
    }
    if (title.find(',') != string::npos) {
        throw SystemException("Title cannot contain a commas");
    }

    Course* course = nullptr;
    if (type == "LEC") {
        course = new LectureCourse(code, title, credits, capacity, lectId);
    } else if (type == "LAB") {
        int labHours = 0;
        labHours = readNumber("Lab hours per week: ");
        course = new LabCourse(code, title, credits, capacity, lectId, labHours);
    } else if (type == "PROJ") {
        string passFail;
        cout << "Pass/fail only? (y/n): ";
        cin >> passFail;
        course = new ProjectCourse(code, title, credits, capacity, lectId, passFail == "y");
    } else {
        throw SystemException("Unknown course type '" + type + "'. Use LEC, LAB or PROJ.");
    }

    // every course needs one timetable slot, or it can never hold a class
    int day = 1;
    int startMin = 540;
    int endMin = 660;
    string location;
    day = readNumber("Day (0=Sun, 1=Mon ... 6=Sat): ");
    startMin = readNumber("Start time in minutes from midnight (540 = 09:00): ");
    endMin = readNumber("End time in minutes from midnight (660 = 11:00): ");
    cout << "Location: ";
    cin >> ws;
    getline(cin, location);
    course->addSlot(TimeSlot(day, startMin, endMin, location));

    string prereq;
    cout << "Prerequisite course code (or NONE): ";
    cin >> prereq;
    if (prereq != "NONE") {
        course->addPrereq(prereq);
    }

    sys.getCourses().add(course);
    sys.saveAll();
    cout << "[Success] Created course " << code << " (" << title << ")\n";
}

// same approach as updateUser: rebuild the course and let the repository replace it
void Administrator::editCourse(string courseCode) {
    UniversitySystem& sys = UniversitySystem::getInstance();

    Course* course = sys.findCourse(courseCode);
    if (course == nullptr) {
        throw SystemException("Course '" + courseCode + "' not found.");
    }

    string title, lectId;
    int capacity = 0;

    cout << "New title: ";
    cin >> ws;
    getline(cin, title);
    capacity = readNumber("New capacity: ");
    cout << "New lecturer ID: ";
    cin >> lectId;

    if (title.find(',') != string::npos) {
        throw SystemException("Title cannot contain a comma, it would break courses.txt.");
    }

    // keep the same subtype, so credits still count the same way
    Course* updated = nullptr;
    LabCourse* lab = dynamic_cast<LabCourse*>(course);
    ProjectCourse* project = dynamic_cast<ProjectCourse*>(course);

    if (lab != nullptr) {
        updated = new LabCourse(courseCode, title, course->getCreditVal(), capacity, lectId, lab->getLabHours());
    } else if (project != nullptr) {
        updated = new ProjectCourse(courseCode, title, course->getCreditVal(), capacity, lectId, project->getIsPassFail());
    } else {
        updated = new LectureCourse(courseCode, title, course->getCreditVal(), capacity, lectId);
    }

    // prerequisites and timetable slots carry across unchanged
    vector<string> prereq = course->getPrerequisites();
    for (int i = 0; i < (int)prereq.size(); i++) {
        updated->addPrereq(prereq[i]);
    }
    vector<TimeSlot> slots = course->getSlots();
    for (int i = 0; i < (int)slots.size(); i++) {
        updated->addSlot(slots[i]);
    }

    sys.getCourses().add(courseCode, updated);   // deletes the old course object
    sys.rebuildStudentTimetables();              // rolls and timetables rebuild from the enrolments
    sys.saveAll();
    cout << "[Success] Updated course " << courseCode << "\n";
}

void Administrator::removeCourse(string courseCode) {
    UniversitySystem& sys = UniversitySystem::getInstance();
    if (!sys.findCourse(courseCode)) {
        throw SystemException("Course '" + courseCode + "' not found.");
    }
    sys.getCourses().remove(courseCode);
    sys.saveAll();
    cout << "[Success] Removed course " << courseCode << "\n";
}

// FR6.1: a plain list of every course held by the system, admin's view, so it
// shows the lecturer and the seats as well. Uses getCourses() and walks the
// whole repository, which is why the admin sees courses that belong to nobody.
void Administrator::listAllCourses() {
    UniversitySystem& sys = UniversitySystem::getInstance();
    vector<Course*> courses = sys.getCourses().all();

    cout << "\n--- All Courses (" << courses.size() << ") ---\n";
    if (courses.empty()) {
        cout << "  (no courses in the system)\n";
        return;
    }

    for (int i = 0; i < (int)courses.size(); i++) {
        Course* c = courses[i];
        Person* lect = sys.getUsers().findById(c->getAssignedLectId());

        // operator<< prints the code, the title and the polymorphic credit value
        cout << "  " << (i + 1) << ". " << *c << "\n";
        cout << "      Lecturer: " << c->getAssignedLectId();
        if (lect != nullptr) {
            cout << " (" << lect->getName() << ")";
        }
        cout << " | Seats: " << c->getEnrolledIds().size() << "/" << c->getCapacity() << "\n";
    }
}

// One course's roll: every enrolled student with the attendance percentage the
// register works out for them. The course average is returned so the enrolment
// report can print it, and it is kept out of generateEnrolmentReport so the
// loop over the courses stays readable.
double Administrator::reportCourseAttendance(Course* course) {
    if (course == nullptr) {
        throw SystemException("No course given to the attendance summary.");
    }

    UniversitySystem& sys = UniversitySystem::getInstance();
    vector<string> enrolled = course->getEnrolledIds();

    if (enrolled.empty()) {
        cout << "  (nobody enrolled)\n";
        return 0.0;
    }

    double totalPct = 0.0;
    for (int i = 0; i < (int)enrolled.size(); i++) {
        Person* p = sys.getUsers().findById(enrolled[i]);
        // the register owns the sessions, so it is the one that works out the percentage
        double pct = sys.getAttendance().attendancePercent(enrolled[i], course->getId());
        totalPct = totalPct + pct;

        cout << "  - " << left << setw(10) << enrolled[i]
             << setw(26) << (p != nullptr ? p->getName() : string("(unknown user)"))
             << "  " << right << fixed << setprecision(1) << setw(6) << pct << "%";
        if (pct < 80.0) {
            cout << "  [!] below the 80% threshold";
        }
        cout << "\n";
    }

    return totalPct / enrolled.size();
}

// FR6.1: every course with its roll, using Course's operator<<, and now each
// student's attendance percentage with the course average underneath
void Administrator::generateEnrolmentReport() {
    UniversitySystem& sys = UniversitySystem::getInstance();

    cout << "\n========================================================\n";
    cout << "                  ENROLMENT REPORT\n";
    cout << "========================================================\n";

    for (Course* c : sys.getCourses().all()) {
        vector<string> enrolled = c->getEnrolledIds();
        cout << "\n" << *c << "\n";
        cout << "  Lecturer: " << c->getAssignedLectId()
             << " | Seats: " << enrolled.size() << "/" << c->getCapacity() << "\n";

        double average = reportCourseAttendance(c);
        if (!enrolled.empty()) {
            cout << "  Course attendance average: "
                 << fixed << setprecision(1) << average << "%\n";
        }
    }
    cout << "========================================================\n\n";
}

string Administrator::toLine() const {
    return "Admin," + getId() + "," + getName() + "," + getPassword();
}