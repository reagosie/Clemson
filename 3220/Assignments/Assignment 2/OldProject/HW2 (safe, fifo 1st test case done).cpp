#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
using namespace std;

int fifo(vector<vector<int> >);
int sjf(vector<vector<int> >);
int rr(vector<vector<int> >);
void serviceSort(vector<int> &vec1, vector<int> &vec2);

//global variables for 3 scheduling functions
char letters[27] = {'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'};
int currTime = 0, currTask = 0;
char currTaskLetter, waitTaskLetter;
int currTaskLength, waitTaskLength;
int currTaskWait = 0, currTaskServ = 0;
int currTaskStart, currTaskEnd;
bool isFinalTask = false, finalTaskComplete = false, thisTaskWaited = false;
vector<int> completionTimes;
vector<int> waitTimes(27, 0);
vector<int> serviceTimes;
vector<int> waitList;

int main(int argc, char *argv[]) {
  vector<vector<int> > tasks;
  int a;
  int i = 0;
  int j = 0;
  int perRow = 0;
  vector<int> temp;

  while(!cin.eof()){
    cin >> a;
    temp.push_back(a);
    perRow++;
    if(perRow == 2){ //have completed a row
      perRow = 0;
      tasks.push_back(temp);
      temp.clear();
    }
  }

/*
//print tasks vector, just for debugging
  cout << endl;
  for(i = 0; i < tasks.size(); i++){
    for(j = 0; j < 2; j++){
      cout << tasks[i][j] << " ";
    }
    cout << endl;
  }
*/

//check flag to see which scheduling type was chosen
  if(strcmp(argv[1], "-fifo") == 0){
    fifo(tasks);
  }
  else if(strcmp(argv[1], "-sjf") == 0){
    sjf(tasks);
  }
  else if(strcmp(argv[1], "-rr") == 0){
    rr(tasks);
  }
  else {
    return 0;
  }
  return 0;
}

int fifo(vector<vector<int> > tasks) {
  int rows = tasks.size();
  int cols = tasks[0].size();

  printf("FIFO scheduling results\n\n");
  printf("time   cpu   ready queue (tid/rst)\n");
  printf("----   ---   ---------------------\n");

//actual scheduling method///////////////////////////////////////////////////////////////////////////////////////////////////////////
  while(!finalTaskComplete){

    if(currTime == tasks[currTask][0] || thisTaskWaited == true){ //if a task is arriving at this time
      if(thisTaskWaited){thisTaskWaited=false;} //reset
      //check for waiting task in next index (but only check if currTask isn't the last element)
      if((currTask+1) < rows){
        if(currTime == tasks[currTask+1][0]){ //if task waiting, put it on the waitList
          waitList[0] = currTask+1;
          waitTaskLetter = letters[currTask+1];
          waitTaskLength = tasks[currTask+1][1];
        }
        else{waitList.push_back(0);} //if not, waitTime is 0
      }

      currTaskStart = currTime;
      currTaskLetter = letters[currTask];
      currTaskLength = tasks[currTask][1];
      while(currTaskLength > 0){ //while currTask is NOT COMPLETED
        if(waitList[0] == 0){ //if nothing waiting, print just currTask
          cout << right << setw(3) << currTime << setw(5) << currTaskLetter << currTaskLength << setw(6) << "--" << endl;
        }
        else{ //if something waiting, print currTask and waiting task
          cout << right << setw(3) << currTime << setw(5) << currTaskLetter << currTaskLength << setw(5) << waitTaskLetter << waitTaskLength << endl;
          //waitTaskLength--;
        }
        currTaskLength--;
        currTime++;
      }

      if(currTaskLength == 0){ //currTask IS COMPLETED!
        if(isFinalTask){finalTaskComplete = true;}
        currTaskEnd = currTime;
        currTaskServ = currTaskEnd - currTaskStart;
        serviceTimes.push_back(currTaskServ);
        completionTimes.push_back(currTime);
        if((currTask+1) < rows){ //if currTask is not the last element, increment
          currTask++;
        }
        if((currTask+1) == rows){isFinalTask = true;}
        if(waitList[0] > 0){
          thisTaskWaited = true;
          waitList[0] = 0;
        }
      }
    }
    else{ //print blank line and increment currTime
      cout << right << setw(3) << currTime << setw(12) << "--" << endl;
      currTime++;
    }
  }
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  printf("\n     arrival service completion response wait\n");
  printf("tid   time    time      time      time   time\n");
  printf("---  ------- ------- ---------- -------- ----\n");
  for(int task = 0; task < rows; task++){
    int responseTime = serviceTimes[task] + waitTimes[task];
    cout << right << setw(2) << letters[task] << setw(7) << tasks[task][0] << setw(8) << tasks[task][1] << setw(10) <<
    completionTimes[task] << setw(10) << (serviceTimes[task]+waitTimes[task]) << setw(7) << waitTimes[task] << endl;
  }

  printf("\nservice wait\n");
  printf(" time   time\n");
  printf("------- ----\n");

  serviceSort(serviceTimes, waitTimes);
  for(int t = 0; t < 3; t++){
    cout << right << setw(4) << serviceTimes[t] << setw(7) << waitTimes[t] << endl;
  }

  return 0;
}

int sjf(vector<vector<int> > tasks) {
  int rows = tasks.size();
  int cols = tasks[0].size();

  printf("SJF(preemptive) scheduling results\n\n");
  printf("time   cpu   ready queue (tid/rst)\n");
  printf("----   ---   ---------------------\n");

//actual scheduling method///////////////////////////////////////////////////////////////////////////////////////////////////////////
  while(!finalTaskComplete){

    if(currTime == tasks[currTask][0] || thisTaskWaited == true){ //if a task is arriving at this time
      if(thisTaskWaited){thisTaskWaited=false;} //reset
      //check for waiting task in next index (but only check if currTask isn't the last element)
      if((currTask+1) < rows){
        if(currTime == tasks[currTask+1][0]){ //if task waiting, put it on the waitList
          waitList[0] = currTask+1;
          waitTaskLetter = letters[currTask+1];
          waitTaskLength = tasks[currTask+1][1];
        }
        else{waitList.push_back(0);} //if not, waitTime is 0
      }

      currTaskStart = currTime;
      currTaskLetter = letters[currTask];
      currTaskLength = tasks[currTask][1];
      while(currTaskLength > 0){ //while currTask is NOT COMPLETED
        if(waitList[0] == 0){ //if nothing waiting, print just currTask
          cout << right << setw(3) << currTime << setw(5) << currTaskLetter << currTaskLength << setw(6) << "--" << endl;
        }
        else{ //if something waiting, print currTask and waiting task
          cout << right << setw(3) << currTime << setw(5) << currTaskLetter << currTaskLength << setw(5) << waitTaskLetter << waitTaskLength << endl;
          //waitTaskLength--;
        }
        currTaskLength--;
        currTime++;
      }

      if(currTaskLength == 0){ //currTask IS COMPLETED!
        if(isFinalTask){finalTaskComplete = true;}
        currTaskEnd = currTime;
        currTaskServ = currTaskEnd - currTaskStart;
        serviceTimes.push_back(currTaskServ);
        completionTimes.push_back(currTime);
        if((currTask+1) < rows){ //if currTask is not the last element, increment
          currTask++;
        }
        if((currTask+1) == rows){isFinalTask = true;}
        if(waitList[0] > 0){
          thisTaskWaited = true;
          waitList[0] = 0;
        }
      }
    }
    else{ //print blank line and increment currTime
      cout << right << setw(3) << currTime << setw(12) << "--" << endl;
      currTime++;
    }
  }
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  printf("\n     arrival service completion response wait\n");
  printf("tid   time    time      time      time   time\n");
  printf("---  ------- ------- ---------- -------- ----\n");
  for(int task = 0; task < rows; task++){
    int responseTime = serviceTimes[task] + waitTimes[task];
    cout << right << setw(2) << letters[task] << setw(7) << tasks[task][0] << setw(8) << tasks[task][1] << setw(10) <<
    completionTimes[task] << setw(10) << (serviceTimes[task]+waitTimes[task]) << setw(7) << waitTimes[task] << endl;
  }

  printf("\nservice wait\n");
  printf(" time   time\n");
  printf("------- ----\n");

  serviceSort(serviceTimes, waitTimes);
  for(int t = 0; t < 3; t++){
    cout << right << setw(4) << serviceTimes[t] << setw(7) << waitTimes[t] << endl;
  }

  return 0;
}

int rr(vector<vector<int> > tasks) {
  int rows = tasks.size();
  int cols = tasks[0].size();

  printf("RR scheduling results (time slice is 1)\n\n");
  printf("time   cpu   ready queue (tid/rst)\n");
  printf("----   ---   ---------------------\n");

//actual scheduling method///////////////////////////////////////////////////////////////////////////////////////////////////////////
  while(!finalTaskComplete){

    if(currTime == tasks[currTask][0] || thisTaskWaited == true){ //if a task is arriving at this time
      if(thisTaskWaited){thisTaskWaited=false;} //reset
      //check for waiting task in next index (but only check if currTask isn't the last element)
      if((currTask+1) < rows){
        if(currTime == tasks[currTask+1][0]){ //if task waiting, put it on the waitList
          waitList[0] = currTask+1;
          waitTaskLetter = letters[currTask+1];
          waitTaskLength = tasks[currTask+1][1];
        }
        else{waitList.push_back(0);} //if not, waitTime is 0
      }

      currTaskStart = currTime;
      currTaskLetter = letters[currTask];
      currTaskLength = tasks[currTask][1];
      while(currTaskLength > 0){ //while currTask is NOT COMPLETED
        if(waitList[0] == 0){ //if nothing waiting, print just currTask
          cout << right << setw(3) << currTime << setw(5) << currTaskLetter << currTaskLength << setw(6) << "--" << endl;
        }
        else{ //if something waiting, print currTask and waiting task
          cout << right << setw(3) << currTime << setw(5) << currTaskLetter << currTaskLength << setw(5) << waitTaskLetter << waitTaskLength << endl;
          //waitTaskLength--;
        }
        currTaskLength--;
        currTime++;
      }

      if(currTaskLength == 0){ //currTask IS COMPLETED!
        if(isFinalTask){finalTaskComplete = true;}
        currTaskEnd = currTime;
        currTaskServ = currTaskEnd - currTaskStart;
        serviceTimes.push_back(currTaskServ);
        completionTimes.push_back(currTime);
        if((currTask+1) < rows){ //if currTask is not the last element, increment
          currTask++;
        }
        if((currTask+1) == rows){isFinalTask = true;}
        if(waitList[0] > 0){
          thisTaskWaited = true;
          waitList[0] = 0;
        }
      }
    }
    else{ //print blank line and increment currTime
      cout << right << setw(3) << currTime << setw(12) << "--" << endl;
      currTime++;
    }
  }
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  printf("\n     arrival service completion response wait\n");
  printf("tid   time    time      time      time   time\n");
  printf("---  ------- ------- ---------- -------- ----\n");
  for(int task = 0; task < rows; task++){
    int responseTime = serviceTimes[task] + waitTimes[task];
    cout << right << setw(2) << letters[task] << setw(7) << tasks[task][0] << setw(8) << tasks[task][1] << setw(10) <<
    completionTimes[task] << setw(10) << (serviceTimes[task]+waitTimes[task]) << setw(7) << waitTimes[task] << endl;
  }

  printf("\nservice wait\n");
  printf(" time   time\n");
  printf("------- ----\n");

  serviceSort(serviceTimes, waitTimes);
  for(int t = 0; t < 3; t++){
    cout << right << setw(4) << serviceTimes[t] << setw(7) << waitTimes[t] << endl;
  }

  return 0;
}

void serviceSort(vector<int> &vec1, vector<int> &vec2) {
  vector<pair<int, int> > doubleVector;
  for(int i = 0; i < vec1.size(); i++){
    doubleVector.push_back(make_pair(vec1[i], vec2[i]));
  }
  sort(doubleVector.begin(), doubleVector.end());
  for(int i = 0; i < vec1.size(); i++){
    vec1[i] = doubleVector[i].first;
    vec2[i] = doubleVector[i].second;
  }
}
