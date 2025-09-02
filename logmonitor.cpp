#include "logmonitor.h"
#include <iostream>
#include <fstream>
using namespace std;

void processLogs(string rulesFile, string logsFile, string outputFile) {
    const int MAX = 100;
    string ruleKeywords[MAX];
    string ruleMessages[MAX];
    string ruleLevels[MAX];
    string ruleIds[MAX];
    int ruleCount = 0;

    // --- Load Rules ---
    ifstream rFile(rulesFile.c_str());
    if (!rFile) {
        cout << "Error: Cannot open " << rulesFile << endl;
        return;
    }

    string line;
    while (ruleCount < MAX && getline(rFile, line)) {
        if (line == "") continue;

        
        // find space positions

        string keyword, level, msg, id;
        size_t firstSpace = line.find(" ");
        size_t secondSpace = line.find(" ", firstSpace + 1);
        size_t lastSpace = line.rfind(" ");

        if (firstSpace != string::npos && secondSpace != string::npos && lastSpace != string::npos) {
            keyword = line.substr(0, firstSpace);
            level   = line.substr(firstSpace + 1, secondSpace - firstSpace - 1);
            msg     = line.substr(secondSpace + 1, lastSpace - secondSpace - 1);
            id      = line.substr(lastSpace + 1);


            // store value in array
            ruleKeywords[ruleCount] = keyword;
            ruleLevels[ruleCount]   = level;
            ruleMessages[ruleCount] = msg;
            ruleIds[ruleCount]      = id;
            ruleCount++;
        }
    }
    rFile.close();

    // --- Process Logs ---
    ifstream lFile(logsFile.c_str());
    if (!lFile) {
        cout << "Error: Cannot open " << logsFile << endl;
        return;
    }

    ofstream out(outputFile.c_str());
    if (!out) {
        cout << "Error: Cannot create " << outputFile << endl;
        return;
    }

    while (getline(lFile, line)) 
    {
        if (line == "") continue;

        // Find timestamp
        size_t start = line.find("[");
        size_t end   = line.find("]");
        if (start == string::npos || end == string::npos) continue;

        string timestamp = line.substr(start + 1, end - start - 1);

        // Find keyword
        size_t pos2 = line.find(":", end);
        if (pos2 == string::npos) continue;

        string keyword = line.substr(end + 2, pos2 - (end + 2));

        bool matched = false;
        for (int i = 0; i < ruleCount; i++) {
            if (keyword == ruleKeywords[i]) {
                out << "TIME:" << timestamp
                    << " LEVEL:" << ruleLevels[i]
                    << " MSG:" << ruleMessages[i]
                    << " ID:" << ruleIds[i] << endl;
                matched = true;
                break;
            }
        }

        if (!matched) {
            out << "TIME:" << timestamp
                << " LEVEL:NO ALERT"
                << " MSG:\"Unrecognized event\""
                << " ID:N/A" << endl;
        }
    }

    lFile.close();
    out.close();
}
