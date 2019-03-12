/*
 * Name: Reagan Leonard
 * Date Submitted: 11/30/18
 * Lab Section: 002
 * Assignment Name: Lab11
 */

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include "search.h"

using namespace std;

int main(int argc, char* argv[]){

  ifstream inputFile;
  inputFile.open("lab11.txt"); //does this need to be just content?

  if(!inputFile){
    cout << "Could not open file!";
    exit(1);
  }

  string query;
  int filecount = 1;

  while(getline(inputFile, query)){
    Searcher searcher;
    //call function to perform search
    cout << "================================================" << endl;
    cout /*<< "Test #" << filecount << ": " */<< query << endl;
    searcher.printMatch(searcher.search(query));
    filecount++;
  }

  inputFile.close();
  return 0;
}
