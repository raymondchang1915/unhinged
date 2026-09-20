#ifndef REPOSITORY_H
#define REPOSITORY_H

#include <map>//for dictionaries 
#include <vector>
#include <string>
#include <fstream>//reading writing
#include <iostream>
#include "Exceptions.h"
using namespace std;

// The <T> means "Type"
template <typename T>
class Repository {
private:
    std::map<string, T*> items; //necessary ones are created when T becomes the relevant class in universitysytem

public:
    Repository() {}

    //adding an item checks if the item exists if so deletes it
    //before adding
    void add(string id, T* item) {
        if (items.find(id) != items.end() && items[id] != item) {//items.end() reaching the end
            delete items[id];//items.find(id) looks through the entire dictionary to find 
        }
        items[id] = item;
    }

    void add(T* item) {//when only the item is provided
        if (item) {
            add(item->getId(),item);//goes back to the above function and gets the ID too
        }
    }

    //remove an item
    bool remove(std::string id) {//tells admin dashboard true/false
        auto it = items.find(id);//auto guesses the variable type
        if (it != items.end()) {
            delete it->second;//second values of the map which is the pointer (first being the ID) clears the heap
            items.erase(it);//clears the record from the map
            return true;
        }
        return false;
    }

    //find an item
    T* findById(std::string id) {//returns a pointer so it can return null if nothing is found otherwise dummy values would be returned
        // If the item exists return it
        if (items.find(id) != items.end()) {
            return items[id];
        }
        return nullptr; //Not found
    }

    //get all the items
    std::vector<T*> all() const {
        std::vector<T*> list;
        // Loop through the map and grab all the values
        for (auto const& pair : items) {
            list.push_back(pair.second);
        }
        return list;
    }

    //file saving
    void save(std::string path) const {
        //output file stream
        std::ofstream file(path);
        
        if (!file.is_open()) {
            throw PersistenceException("Error: Could not create file " + path);
        }

        // Loop through everything in our map and ask it to turn itself into a string!
        for (auto const& pair : items) {
            file << pair.second->toLine() <<endl;
        }
        
        file.close();
    }

    //loading from file
    void load(std::string path, T* (*fromLineFactory)(std::string)) {
        //input file stream
        std::ifstream file(path);
        
        if (!file.is_open()) {
            throw FileNotFoundException(path);
        }

        std::string line;
        int lineNo=1;
        
        // Read the file line by line
        while (std::getline(file,line)) {
            if (line.empty()) { lineNo++; continue; } // Skip empty lines

            // Use the factory function (like Person::fromLine) to turn the text into an object
            T* newItem = fromLineFactory(line);
            
            if (newItem==nullptr) {
                file.close();
                throw CorruptDataException(path,lineNo);
            }
            
            add(newItem->getId(),newItem);
            lineNo++;
        }
        
        file.close();
    }

    void clear() {
        for (auto const& pair : items) {
            delete pair.second;
        }
        items.clear();
    }
    
    // Destructor to clean up memory
    ~Repository() {
        clear();
    }
};

#endif