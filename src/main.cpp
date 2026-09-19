#include <iostream>
#include <string>
#include <vector>

// Include Domain Model and Entities
#include "../include/Person.h"
#include "../include/Student.h"
#include "../include/Lecturer.h"
#include "../include/Administrator.h"
#include "../include/Course.h"
#include "../include/Enrolment.h"
#include "../include/Exceptions.h"
#include "../include/UniversitySystem.h"

using namespace std;

// This is the Menu class from the approved UML class diagram
class Menu{
private:
    // every read goes through these two, so closed input ends the program
    // instead of spinning the menu forever
    bool readChoice(int& choice){
        if (cin >> choice) {
            return true;
        }
        if (cin.eof()) {
            return false;
        }
        cin.clear();
        string discard;
        getline(cin, discard);
        cout<<"Invalid input. Please enter a number.\n";
        choice = -1;
        return true;
    }

    bool readWord(const string& prompt, string& out){
        cout<<prompt;
        return (bool)(cin >> out);
    }

public:
    void handleException(const SystemException& e){
        cout<<"\n========================================================\n";
        cout<<" SYSTEM ALERT: \n";
        cout<<" " << e.what() << "\n";
        cout<<"========================================================\n\n";
    }

    // ---------------- STUDENT ----------------
    void studentMenu(Student* student){
        UniversitySystem& sys = UniversitySystem::getInstance();

        bool first = true;   // run() already printed the dashboard once, polymorphically
        while (true){
            if (!first) student->showDashboard();
            first = false;
            cout<<"Enter your choice (1-5): ";

            int choice = 0;
            if (!readChoice(choice)) return;
            if (choice == 5) return;

            try {
                string courseCode;
                if (choice == 1){
                    if (!readWord("Enter course code to enrol in: ", courseCode)) return;
                    student->enrol(courseCode);
                } else if (choice == 2){
                    if (!readWord("Enter course code to drop: ", courseCode)) return;
                    student->drop(courseCode);
                } else if (choice == 3){
                    cout<<"\n--- My Timetable ---\n" << student->getTimetable();
                } else if (choice == 4){
                    cout<<"\n--- My Attendance ---\n";
                    vector<Enrolment*> mine = sys.getEnrolmentsForStudent(student->getId());
                    if (mine.empty()){
                        cout<<"  (not enrolled in anything yet)\n";
                    }
                    for (Enrolment* e : mine){
                        double pct = sys.getAttendance().attendancePercent(student->getId(), e->getCourseId());
                        cout<<"  " << e->getCourseId() << ": " << (int)pct << "%"
                            << (pct < 80.0 ? "  [!] below 80%" : "") << "\n";
                    }
                } else if (choice != -1){
                    cout<<"Invalid choice. Please enter 1 to 5.\n";
                }
            } catch (const SystemException& e){
                handleException(e);
            }
        }
    }

    // ---------------- LECTURER ----------------
    void lecturerMenu(Lecturer* lecturer){
        UniversitySystem& sys = UniversitySystem::getInstance();

        bool first = true;   // run() already printed the dashboard once, polymorphically
        while (true){
            if (!first) lecturer->showDashboard();
            first = false;
            cout<<"Enter your choice (1-6): ";

            int choice = 0;
            if (!readChoice(choice)) return;
            if (choice == 6) return;

            try {
                string courseCode, sessionId;
                if (choice == 1){
                    cout<<"\n--- My Courses ---\n";
                    for (Course* c : sys.getCourses().all()){
                        if (c->getAssignedLectId() == lecturer->getId()){
                            cout<<"  " << *c << "\n";
                        }
                    }
                } else if (choice == 2){
                    if (!readWord("Enter course code: ", courseCode)) return;
                    lecturer->viewEnrolmentList(courseCode);
                } else if (choice == 3){
                    if (!readWord("Enter course code to open a session for: ", courseCode)) return;
                    Course* course = sys.findCourse(courseCode);
                    if (!course) throw SystemException("Course '" + courseCode + "' not found.");
                    if (course->getSlots().empty()) throw SystemException("Course " + courseCode + " has no timetabled slot.");
                    lecturer->openSession(courseCode, course->getSlots()[0], 10);
                } else if (choice == 4){
                    if (!readWord("Enter session ID to close: ", sessionId)) return;
                    lecturer->closeSession(sessionId);
                } else if (choice == 5){
                    if (!readWord("Enter course code for the report: ", courseCode)) return;
                    Course* course = sys.findCourse(courseCode);
                    if (!course) throw SystemException("Course '" + courseCode + "' not found.");
                    sys.getAttendance().courseAttendanceReport(courseCode);
                } else if (choice != -1){
                    cout<<"Invalid choice. Please enter 1 to 6.\n";
                }
            } catch (const SystemException& e){
                handleException(e);
            }
        }
    }

    // ---------------- ADMINISTRATOR ----------------
    void adminMenu(Administrator* admin){
        bool first = true;   // run() already printed the dashboard once, polymorphically
        while (true){
            if (!first) admin->showDashboard();
            first = false;
            cout<<"Enter your choice (1-4): ";

            int choice = 0;
            if (!readChoice(choice)) return;
            if (choice == 4) return;

            try {
                string id;
                if (choice == 1){
                    if (!readWord("Enter user ID to remove: ", id)) return;
                    admin->removeUser(id);
                } else if (choice == 2){
                    if (!readWord("Enter course code to remove: ", id)) return;
                    admin->removeCourse(id);
                } else if (choice == 3){
                    admin->generateEnrolmentReport();
                } else if (choice != -1){
                    cout<<"Invalid choice. Please enter 1 to 4.\n";
                }
            } catch (const SystemException& e){
                handleException(e);
            }
        }
    }

    void run(){
        cout<<"\n=================================================================\n";
        cout<<"  UNIVERSITY COURSE REGISTRATION, TIMETABLE & ATTENDANCE SYSTEM\n";
        cout<<"                      CO2203 - The Unhinged\n";
        cout<<"=================================================================\n";

        UniversitySystem& sys = UniversitySystem::getInstance();
        sys.initialize("data");

        while (true){
            cout<<"\n========================================================\n";
            cout<<"                     MAIN MENU\n";
            cout<<"========================================================\n";
            cout<<" 1. Log In (Student, Lecturer, Administrator)\n";
            cout<<" 2. Exit\n";
            cout<<"--------------------------------------------------------\n";
            cout<<"Enter your choice (1-2): ";

            int choice = 0;
            if (!readChoice(choice)){
                // input closed (Ctrl+Z / piped file ended): save and leave
                sys.saveAll();
                cout<<"\nInput ended. All data saved. Goodbye!\n";
                break;
            }
            if (choice == -1) continue;

            if (choice == 2) {
                sys.saveAll();
                cout <<"\nAll data saved. System shutting down safely. Goodbye!\n";
                break;
            }

            try {
                if (choice == 1) {
                    cout<<"\n------------------- USER LOGIN -------------------\n";
                    string id, pw;
                    if (!readWord("Enter User ID: ", id)) continue;
                    if (!readWord("Enter Password: ", pw)) continue;

                    Person* user = sys.getUsers().findById(id);
                    if (!user){
                        throw UserNotFoundException(id);
                    }

                    if (!user->login(pw)){
                        throw InvalidCredentialsException();
                    }

                    cout<< "\n[Success] Authentication successful! Welcome, " << user->getName() <<endl;

                    // POLYMORPHIC DISPATCH (FR1.3):
                    // Calls showDashboard() on the base Person pointer without if/else on role strings
                    user->showDashboard();

                    // the role decides which menu handles the choices after that
                    if (Student* s = dynamic_cast<Student*>(user)){
                        studentMenu(s);
                    } else if (Lecturer* l = dynamic_cast<Lecturer*>(user)){
                        lecturerMenu(l);
                    } else if (Administrator* a = dynamic_cast<Administrator*>(user)){
                        adminMenu(a);
                    }
                    cout<<"\n[Logged out]\n";
                } else{
                    cout<<"Invalid choice. Please enter 1 or 2"<<endl;
                }
            } catch (const SystemException& e) {
                handleException(e);
            }
        }
    }
};

int main() {
    Menu systemMenu;
    systemMenu.run();
    return 0;
}
