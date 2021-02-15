#include <string>
#include <vector>

using namespace std;

#ifndef TRACE_ENTRY_H
#define TRACE_ENTRY_H

class TraceEntry
{
    public:
        int time;
        string task_id;
        string readyQueue;

        TraceEntry(int x, string y, string z)
        {
            time = x;
            task_id = y;
            readyQueue = z;
        }  
};
#endif