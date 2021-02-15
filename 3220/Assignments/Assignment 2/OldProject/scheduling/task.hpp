#include <string>

#ifndef TASK_H
#define TASK_H


class Task
{
    public:
        char task_id;
        int
            arrival_time,
            service_time,
            remaining_time,
            completion_time,
            response_time,
            wait_time;
        std::string task_id_str;
        
    Task()
    {
        task_id = '\0';
        arrival_time = 0;
        service_time = 0;
        remaining_time = 0;
        completion_time = 0;
        response_time = 0;
        wait_time = 0;
        task_id_str = "  ";
    }

    Task(char a, int b, int c)
    {
        task_id = a;
        arrival_time = b;
        service_time = c;
        remaining_time = service_time;
        completion_time = 0;
        response_time = 0;
        wait_time = 0;
        task_id_str += task_id + std::to_string(remaining_time);
    }
};
#endif