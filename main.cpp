#include "logmonitor.h"
#include <iostream>
using namespace std;

int main() 
{
    string rulesFile = "rules.txt";
    string logsFile = "log.txt";
    string outputFile = "result.log";

    processLogs(rulesFile, logsFile, outputFile);

    cout << "Log processing completed. Check result.log" << endl;
    return 0;
}

