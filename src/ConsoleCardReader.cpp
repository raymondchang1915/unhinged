#include "ConsoleCardReader.h"
#include <iostream>
using namespace std;

//Since there is no physical card being read, we simulate it by manually typing in the details

void ConsoleCardReader::beginSession() {
    cout << "========================================\n";
    cout << "  USB CARD READER STATION READY\n";
    cout << "  Students can tap cards (enter UID/Student ID)\n";
    cout << "  Type 'stop' or 'exit' when done.\n";
    cout << "========================================\n";
}

CaptureEvent ConsoleCardReader :: captureNext(){
    string uid;
    cout<<"[Card Tap Station] Tap card (UID/Student ID): ";
    cin>>uid;

    //Simulates an exit
    if (uid=="stop" || uid=="STOP" || uid=="exit" || uid=="EXIT"){
        return CaptureEvent("STOP"," ");
    }

    return CaptureEvent(uid,"live_tap");
}

void ConsoleCardReader::endSession(){
    cout<<"---USB CARD READER SESSION ENDED---"<<endl;
}