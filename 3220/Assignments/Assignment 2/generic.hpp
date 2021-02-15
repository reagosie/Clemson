#include <string>
#include <vector>
#include <iostream>
#include <stdio.h>
#include <algorithm>
using namespace std;

#ifndef GENERIC_H
#define GENERIC_H

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class task{
  public:
    char tid; //task ID
    int
      arrTime, //arrival time
      servTime, //service time
      remTime, //remaining time
      compTime, //completion time
      respTime, //response time
      waitTime; //wait time
    string tidStr; //task ID (as a string)

    //Generic constructor
    task(){
      tid = '\0';
      arrTime = 0;
      servTime = 0;
      remTime = 0;
      compTime = 0;
      respTime = 0;
      waitTime = 0;
      tidStr = "  ";
    }

    task(char a, int b, int c){
      tid = a;
      arrTime = b;
      servTime = c;
      remTime = servTime;
      compTime = 0;
      respTime = 0;
      waitTime = 0;
      tidStr += tid + to_string(remTime);
    }
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class traceTask{
  public:
    int time;
    string tid;
    string waitList;

    traceTask(int a, string b, string c){
      time = a;
      tid = b;
      waitList = c;
    }
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class schedulingMethod{
  public:
    vector<task> tasks;
    vector<task*> waitList;
    vector<traceTask> trace;
    vector<task> sortedTasks;

    int time = 0;
    task *currTask = NULL;
    bool taskComplete;

    //Generic constructor
    schedulingMethod();

    void startWorking();
    void preemptMethod();
    void printOutput();
    bool checkIfAllTasksDone();
    string convertWaitListToString();
    void sortTasks();

    //These functions will be different in each scheduling method
    virtual void runCurrTask(){};
    virtual void checkForNewTasks(){};
    virtual void rrTimeSlice(){};
};

//When reading in a file
schedulingMethod::schedulingMethod(){
  int arrTime, servTime;
  char tid = 'A';

  //Read in arrival times and service times of each task
  while (cin >> arrTime >> servTime){
    //Create individual tasks with values read from input file and give them their own tid
    task newTask(tid++, arrTime, servTime);
    //Put the newly made task in our tasks vector
    tasks.push_back(newTask);
  }
}

bool schedulingMethod::checkIfAllTasksDone(){
  int i;
  bool allComplete = true;

  for(i = 0; i < tasks.size(); i++){
    //If any tasks have any remaining time left, then not all tasks are complete
    if(tasks.at(i).remTime != 0){
      allComplete = false;
      break;
    }
  }

  return allComplete;
}

void schedulingMethod::startWorking(){
  do{
    //Must check for new tasks each unit of time
    checkForNewTasks();

    taskComplete = false;

    //Trace tasks so that we can print results in table later
    traceTask tTask(time, (currTask==NULL) ? " " : currTask->tidStr, convertWaitListToString());
    trace.push_back(tTask);

    //If there is a currTask, then run it
    if (currTask != NULL){
      runCurrTask();
    }

    //Just for rr method
    if(!taskComplete){
      rrTimeSlice();
    }

    //Increment time
    time++;
  } while(!checkIfAllTasksDone());

  //Print table from trace vector
  printOutput();
}

string schedulingMethod::convertWaitListToString(){
  int i;
  string tidString;

  if(waitList.size() > 0){
    for(i = 0; i < waitList.size(); i++){
      //If waitList has more than one task, then put a comma after each task
      if(waitList.size() > 1){
        tidString += waitList.at(i)->tidStr + ",";
      }
      //Else just append the task itself
      else{
        tidString += waitList.at(i)->tidStr + " ";
      }
    }
    tidString.resize(tidString.size() - 1);
  }
  //If no tasks on waitList, then append --
  else{
    tidString = "--";
  }
  return tidString;
}

void schedulingMethod::printOutput(){
  int i;

  cout << "time   cpu   ready queue (tid/rst)" << endl;
  cout << "----   ---   ---------------------" << endl;

  //This is where we print each line of our main progress table
  for (i = 0; i < trace.size(); i++){
    cout << setw(3) << trace.at(i).time << setw(6) << trace.at(i).tid << "    " << trace.at(i).waitList << endl;
  }

  cout << endl << "     arrival service completion response wait" << endl;
  cout << "tid   time    time      time      time   time" << endl;
  cout << "---  ------- ------- ---------- -------- ----" << endl;

  //This is where we print the times of each task
  task taskI;
  for (i = 0; i < tasks.size(); i++){
    taskI = tasks.at(i);
    cout << setw(2) << taskI.tid << setw(7) << taskI.arrTime << setw(8) << taskI.servTime << setw(10) <<
      taskI.compTime << setw(10) << taskI.respTime << setw(7) << taskI.waitTime << endl;
  }
  cout << endl;

  //Sort tasks so we can print servTimes and waitTimes in order of servTimes
  sortTasks();
  cout << "service wait" << endl;
  cout << " time   time" << endl;
  cout << "------- ----" << endl;

  for (i = 0; i < tasks.size(); i++){
    taskI = tasks.at(i);
    cout << setw(4) << taskI.servTime << setw(7) << taskI.waitTime << endl;
  }
}

void schedulingMethod::sortTasks(){
  sort(tasks.begin(), tasks.end(), [](const task& a, const task& b){
    //First, compare the servTimes of each task
    if(a.servTime != b.servTime){
      return a.servTime < b.servTime;
    }
    //If servTimes are equal, use waitTimes as tiebreaker
    return a.waitTime < b.waitTime;
  });
}
#endif
