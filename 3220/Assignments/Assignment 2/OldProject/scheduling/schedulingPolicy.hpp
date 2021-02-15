#include <vector>
#include "task.hpp"
#include <iostream>
#include <stdio.h>
#include "traceEntry.hpp"
#include <algorithm>

#ifndef SCHEDULING_POLICY_H
#define SCHEDULING_POLICY_H

using namespace std;

class SchedulingPolicy
{
    public:
        vector<Task> taskList;
        vector<Task*> readyQueue;
        vector<TraceEntry> trace;
        vector<Task> sortedTasks;
        
        int time = 0;
        Task *cpu = NULL;
        bool taskComplete;
        
        // Constructor
        SchedulingPolicy();


        // Method declarations
        void beginRunningTasks();
        void preemptMethod();
        void printSummary();
        bool allTasksComplete();
        string readyQueueToString();
        void sortTaskAscending();

        // Policy specific -> will be defined in policy classes
        virtual void runCurrentTask(){};
        virtual void checkForArrivals(){};
        virtual void preemptOnTimeSlice(){};
};

// Constructor to use when reading a file in
SchedulingPolicy::SchedulingPolicy()
{
    int arrival_time, service_time;
    char task_id = 'A';

    // First column of file contains arrival time; second contains service time
    while (cin >> arrival_time >> service_time)
    {
        // Create task with initial values read from file and unique task ID
        Task vectorEntry(task_id++, arrival_time, service_time);

        // Put the task onto vector
        taskList.push_back(vectorEntry);
    }
}

bool SchedulingPolicy::allTasksComplete()
{
    int i;
    bool allComplete = true;

    for (i = 0; i < (int)taskList.size(); i++)
    {
        if (taskList.at(i).remaining_time != 0)
        {
            allComplete = false;
            break;
        }
    }

    return allComplete;
}

void SchedulingPolicy::beginRunningTasks()
{
    do
    {
        // Each new time tick must check for the arrival of a new task
        checkForArrivals();

        // For round robin only
        if (!taskComplete)
            preemptOnTimeSlice();

        taskComplete = false;
        

        // Trace progress for printing schedule results here
        TraceEntry tEntry(time, (cpu==NULL)?" ":cpu->task_id_str, readyQueueToString());
        trace.push_back(tEntry);

        // If there is a task on the cpu, run it
        if (cpu != NULL)
            runCurrentTask();

        time++;
    } while (!allTasksComplete());

    // Print results
    printSummary();

}

string SchedulingPolicy::readyQueueToString()
{
    int i;
    string stringOfIDs;

    if ((int)readyQueue.size() > 0)
    {
        for (i = 0; i < (int)readyQueue.size(); i++)
        {
            // if list is greater than two characters, then use a comma
            if ((int)readyQueue.size() > 1)
            {
                stringOfIDs += readyQueue.at(i)->task_id_str + ",";
            }
            else
            {
                stringOfIDs += readyQueue.at(i)->task_id_str + " ";
            }
        }
        stringOfIDs.resize(stringOfIDs.size() - 1);
    }
    else
    {
        stringOfIDs = "--";
    }
    
    return stringOfIDs;
}

void SchedulingPolicy::printSummary()
{
    int i;

    // Print scheduling outcome
    printf("time\tcpu\tready queue(tid/rst)\n");
    printf("----\t---\t---------------------\n");

    for (i = 0; i < (int)trace.size(); i++)
    {
        cout << trace.at(i).time << "\t" << trace.at(i).task_id << "\t" << trace.at(i).readyQueue << endl;
    }
    printf("\n");

    // Print task stats
    printf("\tarrival\tservice\tcompletion response wait\n");
    printf("tid\t time\t time\t  time\t     time   time\n");
    printf("---\t-------\t-------\t---------- -------- ----\n");

    Task temp;
    for (i = 0; i < (int)taskList.size(); i++)
    {
        temp = taskList.at(i);

        printf(" %c \t   %2d      %2d       %2d       %2d      %2d \n",
            temp.task_id, temp.arrival_time, temp.service_time, temp.completion_time,
            temp.response_time, temp.wait_time
        );
    }
    printf("\n");

    sortTaskAscending();

    // Print ascending service/wait times
    printf("service\twait\n");
    printf(" time\ttime\n");
    printf("------- ----\n");

    for (i = 0; i < (int)taskList.size(); i++)
    {
        temp = taskList.at(i);

        printf("  %2d\t %2d\n", temp.service_time, temp.wait_time);
   }
}

void SchedulingPolicy::sortTaskAscending()
{
    sort(taskList.begin(), taskList.end(), [](const Task& x, const Task& y)
    {
        // Compare service times first
        if (x.service_time!=y.service_time)
            return x.service_time < y.service_time;
        
        // If service times are the same, use wait times
        return x.wait_time < y.wait_time;
    });
}
#endif