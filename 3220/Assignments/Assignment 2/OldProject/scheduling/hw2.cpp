#include <stdio.h>
#include <string.h>
#include <string>
#include <iostream>
#include "fifoPolicy.hpp"
#include "sjfPolicy.hpp"
#include "rrPolicy.hpp"
#include "task.hpp"
#include "schedulingPolicy.hpp"
#include "traceEntry.hpp"

using namespace std;

int main (int argc, char **argv)
{
    // Quick loop to find flag location and schedule type
    string scheduleType;
    int i;

    for (i = 0; i < argc; i++)
    {
        // Make sure the arguement is at least 2 characters
        if (strlen(argv[i]) > 1)
        {
            if (argv[i][0] == '-')
            {
                scheduleType = argv[i];
                scheduleType = scheduleType.substr(1);
            }
        }
    }

    // Convert input flag to all upper case to make flag case insensitive
    for (i = 0; i < (int)scheduleType.length(); i++)
    {
        scheduleType[i] = tolower(scheduleType[i]);
    }

    // Instantiate scheduling policy object based on command line input
    if (scheduleType == "fifo")
    {
        FifoPolicy scheduler;
        scheduler.beginRunningTasks();
    }
    else if (scheduleType == "sjf")
    {
        SJFPolicy scheduler;
        scheduler.beginRunningTasks();
    }
    else if (scheduleType == "rr")
    {
        RRPolicy scheduler;
        scheduler.beginRunningTasks();
    }
    else
    {
        cout << "Not a valiid scheduling type: " << scheduleType;
        cout << "\nExiting...\n";
        return 0;
    }

    return 0;
}
