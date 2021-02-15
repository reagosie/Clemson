#include "task.hpp"
#include "schedulingPolicy.hpp"

#ifndef RR_POLICY_H
#define RR_POLICY_H

class RRPolicy : public SchedulingPolicy
{
    public:
        // Method declarations
        void beginRunningTasks()
        {
            SchedulingPolicy::beginRunningTasks();
        }

        void printSummary()
        {
            printf("RR Scheduling Results (time slide is 1).\n");
            SchedulingPolicy::printSummary();
        }

        bool allTasksComplete()
        {
            return SchedulingPolicy::allTasksComplete();
        }

        void runCurrentTask();
        void Test();
        void checkForArrivals();
        void preemptOnTimeSlice();
};

void RRPolicy::checkForArrivals()
{
    int i;

    for (i = 0; i < (int)taskList.size(); i++)
    {
        // Check to see if there is an arrival
        if (taskList.at(i).arrival_time == time)
        {
            // If no task running, start arrival
            if (cpu == NULL)
            {
                cpu = &taskList.at(i);
            }
            else
            {
                readyQueue.push_back(&taskList.at(i));
            }
        }
    }

    // Each item on the ready queue should be incremented every iteration that it
    // remains on the queue
    for (i = 0; i < (int)readyQueue.size(); i++)
    {
        readyQueue.at(i)->wait_time++;
    }
}

void RRPolicy::runCurrentTask()
{
    cpu->remaining_time--;    
    cpu->task_id_str = (cpu->task_id_str).substr(0,1) + std::to_string(cpu->remaining_time);

    
    // When task is complete...
    if (cpu->remaining_time == 0)
    {  
        // Mark completion time and calculate response and wait time
        cpu->completion_time = time+1; // note: task is finished AFTER current tick
        cpu->response_time = cpu->completion_time - cpu->arrival_time;
        cpu->wait_time = cpu->response_time - cpu->service_time;

        //TEST: 
        taskComplete = true;

        // See if there is a task on ready queue
        if ((int)readyQueue.size() > 0)
        {
            cpu = readyQueue.front();
            readyQueue.erase(readyQueue.begin());
        }
        else
        {
            cpu = NULL;
        }
    }
}

void RRPolicy::preemptOnTimeSlice()
{
    // If cpu has task and there are task waiting on ready list
    if (cpu != NULL && (int)readyQueue.size() > 0)
    {
        // Put current task back onto ready list
        readyQueue.push_back(cpu);

        // Start next task
        cpu = readyQueue.front();
        readyQueue.erase(readyQueue.begin());
    }
}
#endif