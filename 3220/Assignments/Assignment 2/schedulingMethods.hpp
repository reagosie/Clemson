#include "generic.hpp"
using namespace std;

#ifndef SCHEDULINGMETHODS_H
#define SCHEDULINGMETHODS_H

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class fifoMethod:public schedulingMethod{
  public:
    void startWorking(){
      schedulingMethod::startWorking();
    }

    void printOutput(){
      cout << "FIFO Scheduling Results" << endl << endl;
      schedulingMethod::printOutput();
    }

    bool checkIfAllTasksDone(){
      return schedulingMethod::checkIfAllTasksDone();
    }

    void runCurrTask();
    void checkForNewTasks();
    void test();
};

void fifoMethod::checkForNewTasks(){
  int i;
  for(i = 0; i < tasks.size(); i++){
    //Check for a new task that has arrived
    if(tasks.at(i).arrTime == time){
      //If there's no task currently running, set currTask to this new task
      if(currTask == NULL){
        currTask = &tasks.at(i);
      }
      //Else, (if there's already a currTask) put this new one on the waitList
      else{
        waitList.push_back(&tasks.at(i));
      }
    }
  }

  //Each waitList task's waitTime is incremented each iteration it's still on the waitList
  for(i = 0; i < waitList.size(); i++){
    waitList.at(i)->waitTime++;
  }
}

void fifoMethod::runCurrTask(){
  currTask->remTime--;
  currTask->tidStr = (currTask->tidStr).substr(0,1) + to_string(currTask->remTime);

  //WHEN CURRTASK IS COMPLETE!
  if(currTask->remTime == 0){
    //Record compTime, respTime, and waitTime for this task to report later
    currTask->compTime = time+1;
    currTask->respTime = currTask->compTime - currTask->arrTime;
    currTask->waitTime = currTask->respTime - currTask->servTime;

    //Check waitList for task
    if(waitList.size() > 0){
      currTask = waitList.front();
      waitList.erase(waitList.begin());
    }
    //If nothing there, then there's no currTask
    else{
      currTask = NULL;
    }
  }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class sjfMethod:public schedulingMethod{
  public:
    void startWorking(){
      schedulingMethod::startWorking();
    }

    void printOutput(){
      cout << "SJF(preemptive) Scheduling Results" << endl << endl;
      schedulingMethod::printOutput();
    }

    bool checkIfAllTasksDone(){
      return schedulingMethod::checkIfAllTasksDone();
    }

    void runCurrTask();
    void test();
    void checkForNewTasks();
    void sortWaitingTasks();
};

void sjfMethod::checkForNewTasks(){
  int i;
  for(i = 0; i < tasks.size(); i++){
    //Check for a new task arriving
    if(tasks.at(i).arrTime == time){
      //If there's no currTask, set currTask to this new task
      if(currTask == NULL){
        currTask = &tasks.at(i);
      }
      //If arriving task is shorter than currTask, put currTask on hold
      else if(currTask->remTime > tasks.at(i).servTime){
        //Put currTask on waitList and sort again
        waitList.push_back(currTask);
        sortWaitingTasks();
        //Set currTask to newly arrived task
        currTask = &tasks.at(i);
      }
      //Else, if arriving task not shorter than currTask
      else{
        //Then put newly arrived task on waitList and sort it
        waitList.push_back(&tasks.at(i));
        sortWaitingTasks();
      }
    }
  }

  //Each waitList task's waitTime is incremented each iteration that it's on the waitList
  for (i = 0; i < waitList.size(); i++){
    waitList.at(i)->waitTime++;
  }
}

void sjfMethod::runCurrTask(){
  currTask->remTime--;
  currTask->tidStr = (currTask->tidStr).substr(0,1) + to_string(currTask->remTime);

  //WHEN CURRTASK IS COMPLETE
  if(currTask->remTime == 0){
    //Record compTime, respTime, and waitTime to report on later
    currTask->compTime = time + 1;
    currTask->respTime = currTask->compTime - currTask->arrTime;
    currTask->waitTime = currTask->respTime - currTask->servTime;

    //Check waitList for task
    if(waitList.size() > 0){
      currTask = waitList.front();
      waitList.erase(waitList.begin());
    }
    //If nothing there, then there's no currTask anymore
    else{
      currTask = NULL;
    }
  }
}

//Function to sort tasks on waitList
void sjfMethod::sortWaitingTasks(){
  sort(waitList.begin(), waitList.end(), [](const task* a, const task* b){
    return a->remTime < b->remTime;
  });
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class rrMethod:public schedulingMethod{
  public:
    void startWorking(){
      schedulingMethod::startWorking();
    }

    void printOutput(){
      cout << "RR Scheduling Results (time slice is 1)" << endl << endl;
      schedulingMethod::printOutput();
    }

    bool checkIfAllTasksDone(){
      return schedulingMethod::checkIfAllTasksDone();
    }

    void runCurrTask();
    void test();
    void checkForNewTasks();
    void rrTimeSlice();
};

void rrMethod::checkForNewTasks(){
  int i;
  for(i = 0; i < tasks.size(); i++){
    //Check for a newly arrived task
    if(tasks.at(i).arrTime == time){
      //If no currTask, make this new task currTask
      if(currTask == NULL){
        currTask = &tasks.at(i);
      }
      //Else, if there is a currTask, put new task on waitList
      else{
        waitList.push_back(&tasks.at(i));
      }
    }
  }

  //Each waitList task's waitTime is incremented each iteration that it is still on the waitList
  for (i = 0; i < waitList.size(); i++){
    waitList.at(i)->waitTime++;
  }
}

void rrMethod::runCurrTask(){
  currTask->remTime--;
  currTask->tidStr = (currTask->tidStr).substr(0,1) + to_string(currTask->remTime);

  //WHEN CURRTASK IS COMPLETE
  if(currTask->remTime == 0){
    //Record compTime, respTime, and waitTime for completed task to report later
    currTask->compTime = time+1;
    currTask->respTime = currTask->compTime - currTask->arrTime;
    currTask->waitTime = currTask->respTime - currTask->servTime;

    taskComplete = true;

    //Check waitList for tasks
    if(waitList.size() > 0){
      currTask = waitList.front();
      waitList.erase(waitList.begin());
    }
    //If waitList empty, then no currTask
    else{
      currTask = NULL;
    }
  }
}

//This function is where we implement the rr time slice as 1
void rrMethod::rrTimeSlice(){
  //If there is a currTask and there are also tasks on the waitList
  if(currTask != NULL && waitList.size() > 0){
    //Put currTask on waitList
    waitList.push_back(currTask);
    //Set currTask to next task on waitList
    currTask = waitList.front();
    waitList.erase(waitList.begin());
  }
}
#endif
