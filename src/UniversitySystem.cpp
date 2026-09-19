#include "../include/UniversitySystem.h"
#include "../include/Student.h"
#include "../include/Lecturer.h"
#include "../include/Administrator.h"
#include "../include/LectureCourse.h"
#include "../include/LabCourse.h"
#include "../include/ProjectCourse.h"
#include "../include/Exceptions.h"

#include <iostream>
#include <fstream>
#include <filesystem>

using namespace std;

UniversitySystem* UniversitySystem::instance =nullptr;//At start of the program sets the shared instance pointer to empty

UniversitySystem::UniversitySystem() : dataDir("data"){}//constructor

UniversitySystem& UniversitySystem::getInstance(){//reference used because no need to copy
    if (!instance) instance = new UniversitySystem();//if the university system is empty create a one otherwise jsut return the isntance
    return *instance;
}
void UniversitySystem::initialize(const string& dir){
    dataDir=dir;//saves the data in the private directory
    std::filesystem::create_directories(dataDir); 

    try {
        loadAll();//loading all the data from the files
    } catch (const FileNotFoundException&){
        // Files do not exist yet; save empty repositories to create clean starter files
        saveAll();
    } catch (const SystemException& e){
        cout<< "[System Alert] "<<e.what()<<endl;
    }
    rebuildStudentTimetables();//all loaded enrolments and others figures out which student is in which course, and builds their weekly calendar schedules
}
//when loading onjects doesnt exist in memory so need the helper functions to read and create respective objects
void UniversitySystem::loadAll(){//load to RAM
    users.load(dataDir + "/users.txt", Person::fromLine);//Person::fromLine take the string and create objects depending on whats inside
    courses.load(dataDir + "/courses.txt", Course::fromLine);//create course objects
    enrolments.load(dataDir + "/enrolments.txt", Enrolment::fromLine);
    attendance.load(dataDir + "/attendance.txt");//AttendanceRegister does it without helper functions
}
//already the objects exist in the memory so they are turned into strings and saved
void UniversitySystem::saveAll(){
    users.save(dataDir + "/users.txt");
    courses.save(dataDir + "/courses.txt");
    enrolments.save(dataDir + "/enrolments.txt");
    attendance.save(dataDir + "/attendance.txt");
}

Student* UniversitySystem::findStudent(const string& id){//turns the generic Person* into a Student* and hands it back
    return dynamic_cast<Student*>(users.findById(id));
}

Course* UniversitySystem::findCourse(const string& code){
    return courses.findById(code);
}
//basically a for loop to get enrollment list for specfic studentid
vector<Enrolment*> UniversitySystem::getEnrolmentsForStudent(const string& studentId){//return a vector full of pointers to enrollments
    vector<Enrolment*> res;//creates an empty list called res at first
    for (Enrolment* e:enrolments.all()) {//e-current enrollment
        if (e && e->getStudentId() == studentId) res.push_back(e);//Does the student ID on records match the student condition if so put in res(double checking data)
    }
    return res;
}

string UniversitySystem::resolveUidToStudentId(const string& uid){
    for (Person* p : users.all()) {//get the master list of all users and look at them one person ('p') at a time
        if (Student* s = dynamic_cast<Student*>(p)){//Try to cast the person into a Student. 

        // If 'p' is a Teacher, this fails and skips to the next person.
            if (s->getCard().getUid() == uid || s->getId() == uid) return s->getId();//Return their actual Student ID if match found
        }
    }
    return uid;//return the original scanned code so the system doesn't crash.
}
//used as a reset 
void UniversitySystem::rebuildStudentTimetables() {
    for (Course* c : courses.all()) {//loop through all the course and remove the students one by one
        for (const string& s : c->getEnrolledIds()) c->removeStudent(s);
    }
    for (Person* p : users.all()) {//loop trhough all users in the system dynamic cast is used to check if the person is a s
        if (Student* s = dynamic_cast<Student*>(p)) s->getTimetable().clear();//if student erase their schedule
    }
    for (Enrolment* e : enrolments.all()) {//goes through enrollment records
        if (!e) continue;//for safety if emtpy skip and go to next one
        Student* s = findStudent(e->getStudentId());//use finder functions to get the student and course poiters
        Course* c = findCourse(e->getCourseId());

        if (s && c) {//safety check to check if the student and course actually exist
            c->addStudent(s->getId());//add student names back to classroom
            for (const TimeSlot& slot : c->getSlots()) //check timeslots the course needs
                {s->getTimetable().addSlot(slot, c->getId());}//write the times into students schedule
        }
    }
}
//used to enrol a student in a course through checklists and throws exceptions
void UniversitySystem::enrolStudent(const string& studentId, const string& courseCode){
    
    Student* stu = findStudent(studentId);
    if (!stu) throw UserNotFoundException(studentId);//check if stuent exists if not throw exception
    
    Course* course = findCourse(courseCode);
    if (!course) throw SystemException("Course '" + courseCode + "' not found.");//check if course exisit if not throw exception


//check if enrolled already
    for (Enrolment* e : getEnrolmentsForStudent(studentId)){
        if (e && e->getCourseId() == courseCode) throw AlreadyEnrolledException(courseCode);//throw exception if already enrolled
    }
    if (course->isFull()) throw CourseFullException(courseCode, course->getCapacity());//throw coursefull exception
//check for prerequisites 
    for (const string& prereq : course->getPrerequisites()){
        bool met = false;//set to false at first below functions will turn true if checks are passed
        for (const string& comp : stu->getCompletedCourses()) {//if completed allowed
            if (comp == prereq) {met = true; break;}
        }
        if (!met) {//if not completed see if they are enrolled
            for (Enrolment* e : getEnrolmentsForStudent(studentId)){
                if (e && e->getCourseId() == prereq) {met=true; break;}
            }
        }//if those above arent met throw exception
        if (!met) throw PrerequisiteNotMetException(courseCode,prereq);
    }

    for (const TimeSlot& newSlot : course->getSlots()){//look at time this course requires
        if (stu->getTimetable().hasClash(newSlot)) {//check if student's timetable shows any overlaps
            TimeSlot clashingSlot;//holds which clashes
            for (const TimeSlot& ex : stu->getTimetable().getSlots()){//looks through student's tiemtbale to find which class
                if (ex && newSlot){clashingSlot = ex; break;}
            }
            throw ClashException(newSlot, clashingSlot);
        }
    }
//if all the checks passed then get info
//date is hardcoded 
    enrolments.add(new Enrolment(studentId, courseCode, "2026-09-17"));
    course->addStudent(studentId);
    for (const TimeSlot& slot : course->getSlots()) stu->getTimetable().addSlot(slot, courseCode);//add the class time to the student's schedule
    saveAll();
    cout<< "[Success] Enrolled in " << courseCode << " (" << course->getTitle() << ")"<<endl;
}
//to remove enrollment
void UniversitySystem::dropStudent(const string& studentId, const string& courseCode){
    //student actually exist check
    Student* stu = findStudent(studentId);
    if (!stu) throw UserNotFoundException(studentId);//exception if not
    //course actually exist check
    Course* course = findCourse(courseCode);
    if (!course) throw SystemException("Course '" + courseCode + "' not found.");//exception if not

    string targetId = "";//blank string to hold the Id
    for (Enrolment* e : getEnrolmentsForStudent(studentId)) {//look through all the classes this student takes
        if (e && e->getCourseId() == courseCode) {targetId = e->getId();break;}//save the record
    }
    if (targetId.empty()) throw SystemException("Student is not enrolled in course " + courseCode + ".");//throw exception

    enrolments.remove(targetId);//delete the record from the master enrollment base

    course->removeStudent(studentId);//remove the studenteID from the teacher classroomrooster

    stu->getTimetable().removeSlotsOf(courseCode);//remove the course from student weekly schedule

    saveAll();
    cout<< "[Success] Dropped course " << courseCode <<endl;
}
