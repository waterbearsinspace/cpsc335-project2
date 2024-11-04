
// =============================================================================
// Names: 
//      Noemi Pantoja Morales
//      Andreia Shin
//      Nick Viste
// Date: 11/06/2024
// Project: Matching Group Schedules
// Purpose: This program takes the schedule (an array list) of two or more 
//  group members, an array of two entries that represent the daily working 
//  periods for each group member and an integer representing the miniumum
//  duration of the meeting required.
//
//  The goal is to find a time slot when all group members are free for a 
//  meeting, considering the provided schedules and the minimum duration 
//  required for the meeting.
// =============================================================================
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

int convertToMinutes(string time) {
    // "H:MM"
    if (time.size() == 4) {
        return (stoi(time.substr(0, 1)) * 60 + stoi(time.substr(2, 2)));
    }
    // "HH:MM"
    else {
        return (stoi(time.substr(0, 2)) * 60 + stoi(time.substr(3, 2)));
    }
}

// assume both schedules are already sorted within themselves
vector<pair<string, string>> combineSchedules(vector<pair<string, string>> person1Schedule, 
    vector<pair<string, string>> person2Schedule) {
    // iterators through the two schedules
    int i = 0;
    int j = 0;

    // combined schedule
    vector<pair<string, string>> combined;

    // add all in order
    while ((i < person1Schedule.size()) && (j < person2Schedule.size())) {
        // if the start of the first schedule is earlier than the start of the second, 
        // push the first schedule
        if (convertToMinutes(person1Schedule[i].first) < 
            convertToMinutes(person2Schedule[j].first)) {
            combined.push_back(person1Schedule[i]);
            i++;
        }

        // otherwise
        else {
            // if they're the same start
            if (convertToMinutes(person1Schedule[i].first) == 
                convertToMinutes(person2Schedule[j].first)) {
                // push the schedule with an earlier end
                if (convertToMinutes(person1Schedule[i].second) < 
                    convertToMinutes(person2Schedule[j].second)) {
                    combined.push_back(person1Schedule[i]);
                    i++;
                }
                else {
                    combined.push_back(person2Schedule[j]);
                    j++;
                }
            }
            // otherwise push the second schedule
            else {
                combined.push_back(person2Schedule[j]);
                j++;
            }
        }
    }

    // add the rest
    while (i < person1Schedule.size()) {
        combined.push_back(person1Schedule[i]);
        i++;
    }
    while (j < person2Schedule.size()) {

        combined.push_back(person2Schedule[j]);
        j++;
    }

    return combined;
}


int main() {
    // read and write files
    ifstream inputFile("input.txt");
    ofstream outputFile("output.txt");

    if (!inputFile.is_open() || !outputFile.is_open()) {
        cerr << "Error: Unable to open input or output file.\n";
    }

    outputFile << "Available slots for the meeting:\n";
    inputFile.close();
    outputFile.close();

    // test values, to be read from input.txt
    vector<pair<string, string>> person1Schedule;
    person1Schedule.push_back({ "7:00", "8:30" });
    person1Schedule.push_back({ "12:00", "13:00" });
    person1Schedule.push_back({ "16:00", "18:00" });
    vector<pair<string, string>> person2Schedule;
    person2Schedule.push_back({ "9:00", "10:30" });
    person2Schedule.push_back({ "12:20", "13:30" });
    person2Schedule.push_back({ "14:00", "15:00" });
    person2Schedule.push_back({ "16:00", "17:00" });

    // test combineSchedules
    vector<pair<string, string>> combined = combineSchedules(person1Schedule, person2Schedule);
    cout << endl;
    cout << "Combined Schedule:" << endl;
    for (int i = 0; i < combined.size(); i++) {
        cout << combined[i].first << " " << combined[i].second << endl;
    }

    return 0;
}