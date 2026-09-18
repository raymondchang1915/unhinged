#include <iostream>
#include <string>

// Include Domain Model and Entities
#include "../include/Person.h"
#include "../include/Student.h"
#include "../include/Lecturer.h"
#include "../include/Administrator.h"
#include "../include/Exceptions.h"
#include "../include/UniversitySystem.h"

using namespace std;

// This is the Menu class from the approved UML class diagram
class Menu{
public:
    void handleException(const SystemException& e){
        cout<<"\n========================================================\n";
        cout<<" SYSTEM ALERT: \n";
        cout<<" " << e.what() << "\n";
        cout<<"========================================================\n\n";
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
            if (!(cin >> choice)){
                cin.clear();
                string discard;
                getline(cin, discard);
                cout<<"Invalid input. Please enter a number.\n";
                continue;
            }

            if (choice == 2) {
                sys.saveAll();
                cout <<"\nAll data saved. System shutting down safely. Goodbye!\n";
                break;
            }

            try {
                if (choice == 1) {
                    cout<<"\n------------------- USER LOGIN -------------------\n";
                    cout<<"Enter User ID: ";
                    string id;
                    cin>>id;

                    cout<<"Enter Password: ";
                    string pw;
                    cin>>pw;

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