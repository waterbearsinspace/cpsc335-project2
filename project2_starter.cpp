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

// Convert "hh:mm" to minutes
int toMinutes(string time) {
    if (time.size() == 4) { // "H:MM"
        return stoi(time.substr(0, 1)) * 60 + stoi(time.substr(2, 2));
    }
    else { // "HH:MM"
        return stoi(time.substr(0, 2)) * 60 + stoi(time.substr(3, 2));
    }
}

// Print a schedule to an output file
void printSched(const vector<pair<string, string>>& sched, ofstream& outFile) {
        outFile << "Availability: \n";
    for (const auto& period : sched) {
        outFile << period.first << " " << period.second << endl;
    }
    outFile << endl; // Separate test cases with a blank line
}

// Function to combine two schedules
vector<pair<string, string>> combineSchedules(const vector<pair<string, string>>& p1sched,
    const vector<pair<string, string>>& p2sched) {
    size_t i = 0, j = 0;
    vector<pair<string, string>> combined, merged;

    // Combine the schedules in sorted order
    while (i < p1sched.size() && j < p2sched.size()) {
        if (toMinutes(p1sched[i].first) < toMinutes(p2sched[j].first)) {
            combined.push_back(p1sched[i++]);
        } else if (toMinutes(p1sched[i].first) == toMinutes(p2sched[j].first)) {
            combined.push_back(toMinutes(p1sched[i].second) < toMinutes(p2sched[j].second) ? p1sched[i++] : p2sched[j++]);
        } else {
            combined.push_back(p2sched[j++]);
        }
    }

    // Add remaining entries
    while (i < p1sched.size()) combined.push_back(p1sched[i++]);
    while (j < p2sched.size()) combined.push_back(p2sched[j++]);

    // Merge overlapping schedules
    pair<string, string> current = combined[0];
    for (size_t k = 1; k < combined.size(); ++k) {
        if (toMinutes(current.second) >= toMinutes(combined[k].first)) {
            current.second = max(current.second, combined[k].second);
        } else {
            merged.push_back(current);
            current = combined[k];
        }
    }
    merged.push_back(current);

    return merged;
}

// Main scheduling function
vector<pair<string, string>> groupSchedule(const vector<vector<pair<string, string>>>& schedules,
    const vector<pair<string, string>>& workingPeriods, int duration) {
    size_t i = 0;
    vector<pair<string, string>> combinedSchedules = schedules[i++];

    while (i < schedules.size()) {
        combinedSchedules = combineSchedules(combinedSchedules, schedules[i++]);
    }

    pair<string, string> logTimes = workingPeriods[0];
    for (const auto& period : workingPeriods) {
        logTimes.first = max(logTimes.first, period.first);
        logTimes.second = min(logTimes.second, period.second);
    }

    vector<pair<string, string>> finalSchedule;
    for (size_t j = 1; j < combinedSchedules.size(); ++j) {
        pair<string, string> current = { combinedSchedules[j - 1].second, combinedSchedules[j].first };
        if (toMinutes(current.first) >= toMinutes(logTimes.first) &&
            toMinutes(current.second) <= toMinutes(logTimes.second) &&
            toMinutes(current.second) - toMinutes(current.first) >= duration) {
            finalSchedule.push_back(current);
        }
    }

    pair<string, string> lastPeriod = { combinedSchedules.back().second, logTimes.second };
    if (toMinutes(lastPeriod.second) - toMinutes(lastPeriod.first) >= duration) {
        finalSchedule.push_back(lastPeriod);
    }

    return finalSchedule;
}

int main() {
    ifstream inFile("input.txt");
    ofstream outFile("output.txt");

    if (!inFile.is_open()) {
        cerr << "Error opening input file" << endl;
        return 1;
    }
    if (!outFile.is_open()) {
        cerr << "Error opening output file" << endl;
        return 1;
    }

    while (true) {
        vector<vector<pair<string, string>>> schedules;
        vector<pair<string, string>> workingPeriods;

        int numSchedules;
        if (!(inFile >> numSchedules)) break; // Exit loop if end of file is reached

        // Read each person's schedule
        for (int i = 0; i < numSchedules; ++i) {
            int numEntries;
            inFile >> numEntries;

            vector<pair<string, string>> schedule;
            for (int j = 0; j < numEntries; ++j) {
                string start, end;
                inFile >> start >> end;
                schedule.push_back({start, end});
            }
            schedules.push_back(schedule);
        }

        int numPeriods;
        inFile >> numPeriods;

        for (int i = 0; i < numPeriods; ++i) {
            string start, end;
            inFile >> start >> end;
            workingPeriods.push_back({start, end});
        }

        int duration;
        inFile >> duration;

        // Generate the final schedule and output to file
        vector<pair<string, string>> results = groupSchedule(schedules, workingPeriods, duration);
        printSched(results, outFile);
    }

    inFile.close();
    outFile.close();

    return 0;
}
