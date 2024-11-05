
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

int toMinutes(string time) {
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
vector<pair<string, string>> combineSchedules(vector<pair<string, string>> p1sched,
    vector<pair<string, string>> p2sched) {
    // iterators through the two schedules
    int i = 0;
    int j = 0;

    // combined schedule
    vector<pair<string, string>> combined;
    vector<pair<string, string>> merged;

    // add all in order
    while ((i < p1sched.size()) && (j < p2sched.size())) {
        // if the start of the first schedule is earlier than the start of the second, 
        // push the first schedule
        if (toMinutes(p1sched[i].first) < toMinutes(p2sched[j].first)) {
            combined.push_back(p1sched[i]);
            i++;
        }

        // otherwise
        else {
            // if they're the same start
            if (toMinutes(p1sched[i].first) == toMinutes(p2sched[j].first)) {
                // push the schedule with an earlier end
                if (toMinutes(p1sched[i].second) <
                    toMinutes(p2sched[j].second)) {
                    combined.push_back(p1sched[i]);
                    i++;
                }
                else {
                    combined.push_back(p2sched[j]);
                    j++;
                }
            }
            // otherwise push the second schedule
            else {
                combined.push_back(p2sched[j]);
                j++;
            }
        }
    }

    // add the rest
    while (i < p1sched.size()) {
        combined.push_back(p1sched[i]);
        i++;
    }
    while (j < p2sched.size()) {
        combined.push_back(p2sched[j]);
        j++;
    }

    // reset iterators
    i = 0;
    j = 1;
    pair<string, string> current = combined[i];

    // merge schedules
    while ((i < combined.size()) && (j < combined.size())) {
        if ((toMinutes(current.first) <= toMinutes(combined[j].first)) &&
            (toMinutes(current.second) >= toMinutes(combined[j].first))) {
            current.second =
                (toMinutes(combined[i].second) > toMinutes(combined[j].second) ?
                    combined[i].second : combined[j].second);
            j++;
            if (j >= combined.size()) {
                merged.push_back(current);
            }
        }
        else {
            merged.push_back(current);
            i = j;
            current = combined[i];
            j++;
        }
    }

    return merged;
}


int main() {
    //// read and write files
    //ifstream inputFile("input.txt");
    //ofstream outputFile("output.txt");

    //if (!inputFile.is_open() || !outputFile.is_open()) {
    //    cerr << "Error: Unable to open input or output file.\n";
    //}

    //outputFile << "Available slots for the meeting:\n";
    //inputFile.close();
    //outputFile.close();

    // test values, to be read from input.txt
    vector<pair<string, string>> p1sched;
    p1sched.push_back({ "7:00", "8:30" });
    p1sched.push_back({ "12:00", "13:00" });
    p1sched.push_back({ "16:00", "18:00" });
    vector<pair<string, string>> p2sched;
    p2sched.push_back({ "9:00", "10:30" });
    p2sched.push_back({ "12:20", "13:30" });
    p2sched.push_back({ "14:00", "15:00" });
    p2sched.push_back({ "16:00", "17:00" });

    // test combineSchedules
    vector<pair<string, string>> combined = combineSchedules(p1sched, p2sched);
    cout << endl;
    cout << "Combined Schedule:" << endl;
    for (int i = 0; i < combined.size(); i++) {
        cout << combined[i].first << " " << combined[i].second << endl;
    }

    return 0;
}