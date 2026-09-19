#include "../include/FileReplayCapture.h"
#include "../include/Exceptions.h"
#include <iostream>
#include <sstream>

using namespace std;

//saves the path to the test file
FileReplayCapture::FileReplayCapture(string path) {
    this->filePath = path;
}
//for testing
void FileReplayCapture::beginSession() {
    fileStream.open(filePath);//open the text file if not found throws the exception
    if (!fileStream.is_open()) {
        throw FileNotFoundException(filePath);
    }
    cout<<"File Replay Started:"<< filePath <<endl;//prints
}

CaptureEvent FileReplayCapture::captureNext() {//checks if the file is closed OR if it reached the (end of file) eof()rnf f file.
    if (!fileStream.is_open() || fileStream.eof()) {
        return CaptureEvent("EOF", "");//returns a special event saying "EOF".
    }

    string line;
    while (getline(fileStream, line)) {//loop grabs the next line from the text file. first argument is te destination second is variable name of line
        if (line.empty() || line[0] == '#') continue; // skip blank lines and comments
        stringstream ss(line);//turns the line into stringstream
        string uid, timestamp;
        getline(ss, uid, ',');//read from ss until find a comma and put that string into UID
        getline(ss, timestamp);//whats left to timestamp
        if (timestamp.empty()) timestamp = "replay_sim"; //if no commas were found fake timestamp so the program doesnt crash
        
        cout << "[FileReplay] Replaying event from file -> UID: " << uid <<endl;
        return CaptureEvent(uid, timestamp);//creats a new object Capture event
    }

    return CaptureEvent("EOF", "");//if while loop reaches the end of file sends EOF
}

void FileReplayCapture::endSession() {
    if (fileStream.is_open()) {
        fileStream.close();//closes the opened session
    }
    cout << "-- File Replay Finished --"<<endl;
}