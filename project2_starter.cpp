
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

// convert "hh:mm" to minutes
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

// function to combine two schedules
// assume both schedules are already sorted within themselves
vector<pair<string, string>> combineSchedules(vector<pair<string, string>> p1sched,
    vector<pair<string, string>> p2sched) {
    // iterators through the two schedules
    int i = 0;
    int j = 0;

    // combined and merged schedules
    // combine and sort both schedules then merge any overlaps
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

    // merge overlapping schedules
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

// the actual scheduling function
vector<pair<string, string>> groupSchedule(vector<vector<pair<string,
    string>>> schedules, vector<pair<string, string>> workingPeriods, int duration) {
    // iterator
    int i = 0;

    // initialize first schedule
    vector<pair<string, string>> combinedSchedules = schedules[i];
    i++;

    // combine all schedules
    while (i < schedules.size()) {
        combinedSchedules = combineSchedules(combinedSchedules, schedules[i]);
        i++;
    }

    // get latest login and earliest logout
    pair<string, string> logTimes = workingPeriods[0];
    for (int i = 0; i < workingPeriods.size(); i++) {
        if (toMinutes(workingPeriods[i].first) > toMinutes(logTimes.first)) {
            logTimes.first = workingPeriods[i].first;
        }
        if (toMinutes(workingPeriods[i].second) < toMinutes(logTimes.second)) {
            logTimes.second = workingPeriods[i].second;
        }
    }

    // generate final schedule
    vector<pair<string, string>> finalSchedule;


    // initialize iterator for inbetweens
    i = 1;
    pair<string, string> current;

    // add valid inbetweens
    while (i < combinedSchedules.size()) {
        current = { combinedSchedules[i - 1].second, combinedSchedules[i].first };
        if ((toMinutes(current.first) >= toMinutes(logTimes.first)) &&
            (toMinutes(current.second) <= toMinutes(logTimes.second)) &&
            (toMinutes(current.second) - toMinutes(current.first) >= duration)) {
            finalSchedule.push_back(current);
        }
        i++;
    }
    // check final unavailable to logout
    current = { combinedSchedules[i - 1].second, logTimes.second };
    if ((toMinutes(current.second) <= toMinutes(logTimes.second)) &&
        (toMinutes(current.second) - toMinutes(current.first) >= duration)) {
        finalSchedule.push_back(current);
    }

    return finalSchedule;
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

    // to be read from files
    vector<vector<pair<string, string>>> schedules;
    vector<pair<string, string>> workingPeriods;


    // test values, to be read from input.txt
    vector<pair<string, string>> p1sched;
    p1sched.push_back({ "7:00", "8:30" });
    p1sched.push_back({ "12:00", "13:00" });
    p1sched.push_back({ "16:00", "18:00" });

    workingPeriods.push_back({ "9:00", "19:00" });

    vector<pair<string, string>> p2sched;
    p2sched.push_back({ "9:00", "10:30" });
    p2sched.push_back({ "12:20", "13:30" });
    p2sched.push_back({ "14:00", "15:00" });
    p2sched.push_back({ "16:00", "17:00" });

    schedules.push_back(p1sched);
    schedules.push_back(p2sched);

    workingPeriods.push_back({ "9:00", "18:30" });



    // test combineSchedules
    vector<pair<string, string>> results = groupSchedule(schedules, workingPeriods, 30);
    for (int i = 0; i < results.size(); i++) {
        cout << results[i].first << " " << results[i].second << endl;
    }

    return 0;
}