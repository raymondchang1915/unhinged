#ifndef REPOSITORY_H
#define REPOSITORY_H

#include <map>//for dictionaries 
#include <vector>
#include <string>
#include <fstream>//reading writing
#include <iostream>
#include "Exceptions.h"

// The <T> means "Type"
template <typename T>
class Repository {
private:
    std::map<std::string, T*> items;

public:
    Repository() {}

    //adding an item checks if the item exists if so deletes it
    //before adding
    void add(std::string id, T* item) {
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

    // --- REMOVE AN ITEM ---
    bool remove(std::string id) {
        auto it = items.find(id);//auto guesses the variable type
        if (it != items.end()) {
            delete it->second;
            items.erase(it);
            return true;
        }
        return false;
    }

    bool exists(std::string id) const {//function only reads data checks if an ID exists
        return items.find(id) != items.end();
    }

    //find an item
    T* findById(std::string id) {
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