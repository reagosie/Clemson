#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <string>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include "schedulingMethods.hpp"
using namespace std;

int main (int argc, char *argv[]){
  //Check if there's enough CLA
  if(argc < 2){
    cout << "Error: Not enough command line arguments." << endl;
    cout << "Must input at least 2 arguments plus an input file." << endl;
    cout << "Please try again" << endl;
  }
  else{
    //Makeshift switch statement for scheduling method choice
    //For each choice, declare the corresponding method class and "startWorking"
    if(strcmp(argv[1], "-fifo") == 0){
      fifoMethod fifo;
      cout << "FIFO Scheduling Results" << endl << endl;
      fifo.startWorking();
    }
    else if(strcmp(argv[1], "-sjf") == 0){
      sjfMethod sjf;
      cout << "SJF(preemptive) Scheduling Results" << endl << endl;
      sjf.startWorking();
    }
    else if(strcmp(argv[1], "-rr") == 0){
      rrMethod rr;
      cout << "RR Scheduling Results (time slice is 1)" << endl << endl;
      rr.startWorking();
    }
    //Error message for any flag other than -fifo, -sjf, or -rr
    else{
      cout << "Error: " << argv[1] << " is not a valid scheduling method" << endl;
      cout << "Scheduling method must be one of the following:" << endl << endl;
      cout << "-fifo" << endl << "-sjf" << endl << "-rr" << endl << endl;
      cout << "Please try again" << endl;
    }
  }
  return 0;
}
