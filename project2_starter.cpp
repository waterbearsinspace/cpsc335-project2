// =============================================================================
// Names: 
//      Noemi Pantoja Morales
//      Andreia Shin
//      Nick Viste
// Date: 11/06/2024
// Project: Matching Group Schedules
// Purpose: This program takes the schedule (as a vector) of two or more 
//  group members, a vector of two entries that represent the daily working 
//  periods for each group member, and an integer representing the miniumum
//  duration of the meeting required.
//
//  The goal is to find any time slots where all group members are free for 
//  a meeting, considering the provided schedules and the minimum duration 
//  required for the meeting.
// =============================================================================
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

// Convert "HH:MM" to minutes
int toMinutes(string time) {
    // "H:MM"
    if (time.size() == 4) {
        return stoi(time.substr(0, 1)) * 60 + stoi(time.substr(2, 2));
    }
    // "HH:MM"
    else {
        return stoi(time.substr(0, 2)) * 60 + stoi(time.substr(3, 2));
    }
}

// Print vector to console for testing purposes
void printVect(const vector<pair<string, string>>& toPrint, string phrase) {
    cout << "START " << phrase << endl;
    for (int i = 0; i < toPrint.size(); i++) {
        cout << toPrint[i].first << " " << toPrint[i].second << endl;
    }
}

// Add a schedule to an output file
void outputSchedule(const vector<pair<string, string>>& sched, ofstream& outFile) {
    // Add all periods of availability
    outFile << "Availability: \n";
    for (const pair<string, string>& period : sched) {
        outFile << period.first << " " << period.second << endl;
    }
    // Separate test cases with a blank line
    outFile << endl;
}

// Function to combine two schedules
vector<pair<string, string>> combineSchedules(
    const vector<pair<string, string>>& p1sched, const vector<pair<string, string>>& p2sched) {
    // Initialize iterators
    size_t i = 0, j = 0;
    // Initialize vectors of the two input schedules combined and sorted
    // and of this result with any overlapping schedules merged
    vector<pair<string, string>> combined, merged;

    // Combine the schedules in sorted order
    while (i < p1sched.size() && j < p2sched.size()) {
        // Push the first schedule if it begins before the second
        if (toMinutes(p1sched[i].first) < toMinutes(p2sched[j].first)) {
            combined.push_back(p1sched[i++]);
        }
        // Otherwise combine the two schedules if they start at the same time
        else if (toMinutes(p1sched[i].first) == toMinutes(p2sched[j].first)) {
            pair<string, string> sameStart = { p1sched[i].first, (toMinutes(p1sched[i].second) > toMinutes(p2sched[j].second) ?
                p1sched[i].second : p2sched[j].second) };
            combined.push_back(sameStart);
            i++;
            j++;
        }
        // Otherwise push the second schedule
        else {
            combined.push_back(p2sched[j++]);
        }
    }

    // Add remaining entries from each schedule
    while (i < p1sched.size()) combined.push_back(p1sched[i++]);
    while (j < p2sched.size()) combined.push_back(p2sched[j++]);

    // Merge overlapping schedules in the combined schedule
    // Initialize the current schedule as the first in the combined vector
    pair<string, string> current = combined[0];
    for (size_t k = 1; k < combined.size(); ++k) {
        // If the current schedule starts after the next schedule begins
        // set the current schedule to end at the later end time of the two
        if (toMinutes(current.second) >= toMinutes(combined[k].first)) {
            current.second = (toMinutes(current.second) > toMinutes(combined[k].second) ?
                current.second : combined[k].second);
        }
        // Otherwise push the current schedule and set the current schedule to the next schedule
        else {
            merged.push_back(current);
            current = combined[k];
        }
    }
    // Push the final current schedule
    merged.push_back(current);

    return merged;
}

// Main scheduling function
vector<pair<string, string>> groupSchedule(const vector<vector<pair<string, string>>>& schedules,
    const vector<pair<string, string>>& workingPeriods, int duration) {
    // Initialize iterator
    size_t i = 0;

    // Initialize the total combined schedules as empty if no schedules are found and return this
    // or as the first schedule and if so, increase iterator
    vector<pair<string, string>> combinedSchedules;
    if (schedules.size() == 0) return { {} };
    else combinedSchedules = schedules[i++];

    // Initialize working period as the first working period
    pair<string, string> logTimes = workingPeriods[0];

    // Of the working periods, set the logTimes as the latest login and earliest logout
    for (const pair<string, string>& period : workingPeriods) {
        logTimes.first = (toMinutes(logTimes.first) > toMinutes(period.first) ?
            logTimes.first : period.first);
        logTimes.second = (toMinutes(logTimes.second) < toMinutes(period.second) ?
            logTimes.second : period.second);
    }

    // Combine the current total combined schedules with the next schedule
    while (i < schedules.size()) {
        combinedSchedules = combineSchedules(combinedSchedules, schedules[i++]);
    }


    // Initialize final schedule of availabile periods
    vector<pair<string, string>> finalSchedule;

    // Create a period from login time to first busy schedule
    // and push if it is valid
    pair<string, string> firstPeriod = { logTimes.first, combinedSchedules.front().first };
    if (toMinutes(firstPeriod.second) - toMinutes(firstPeriod.first) >= duration) {
        finalSchedule.push_back(firstPeriod);
    }

    for (size_t j = 1; j < combinedSchedules.size(); ++j) {
        // If the period between unavailable schedules is within the log times 
        // and is of valid duration, push it as a period of availability
        pair<string, string> current = { combinedSchedules[j - 1].second, combinedSchedules[j].first };
        if (toMinutes(current.first) >= toMinutes(logTimes.first) &&
            toMinutes(current.second) <= toMinutes(logTimes.second) &&
            toMinutes(current.second) - toMinutes(current.first) >= duration) {
            finalSchedule.push_back(current);
        }
    }

    // Create a period from the last busy schedule to the logout time
    // and push if it is valid
    pair<string, string> lastPeriod = { combinedSchedules.back().second, logTimes.second };
    if (toMinutes(lastPeriod.second) - toMinutes(lastPeriod.first) >= duration) {
        finalSchedule.push_back(lastPeriod);
    }

    return finalSchedule;
}

int main() {
    // Initialize input and output streams
    ifstream inFile("input.txt");
    ofstream outFile("output.txt");

    // Return errors if unable to properly read/write files
    if (!inFile.is_open()) {
        cerr << "Error opening input file" << endl;
        return 1;
    }
    if (!outFile.is_open()) {
        cerr << "Error opening output file" << endl;
        return 1;
    }

    // Go through each test case and provide availabilities
    while (true) {
        // Initialize all schedules and workingPeriods, to be read from input
        vector<vector<pair<string, string>>> schedules;
        vector<pair<string, string>> workingPeriods;

        // Take in the number of schedules for this case,
        // exiting if no more cases are found
        int numSchedules;
        if (!(inFile >> numSchedules)) break;

        // Read each person's schedule
        for (int i = 0; i < numSchedules; ++i) {
            // Read number of busy schedule entries for current person
            int numEntries;
            inFile >> numEntries;

            // Read each schedule for the current person
            vector<pair<string, string>> schedule;
            for (int j = 0; j < numEntries; ++j) {
                string start, end;
                inFile >> start >> end;
                schedule.push_back({ start, end });
            }

            // Push the current person's schedule to the vector of all schedules
            schedules.push_back(schedule);
        }

        // Take in the number of working periods for this case
        int numPeriods;
        inFile >> numPeriods;

        // Read each working period and push to the vector of all working periods
        for (int i = 0; i < numPeriods; ++i) {
            string start, end;
            inFile >> start >> end;
            workingPeriods.push_back({ start, end });
        }

        // Read the desired minimum duration for meetings
        int duration;
        inFile >> duration;

        // Generate the final schedule and output to file
        vector<pair<string, string>> results = groupSchedule(schedules, workingPeriods, duration);
        outputSchedule(results, outFile);
    }

    // Close input and output streams
    inFile.close();
    outFile.close();

    return 0;
}
