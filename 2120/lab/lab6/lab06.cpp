#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include "nearest_neighbor.h"


//please add tests to lab06.txt
//there is no need to edit anything in this file
using namespace std;

void fill_nearest_neighbor(ifstream& infile, nearest_neighbor& near_neighor);

int main(){
    ifstream infile("lab06.txt");
    do{
        nearest_neighbor near_neighor;
        fill_nearest_neighbor(infile, near_neighor);
        near_neighor.find_closest_points();
        cout << endl << endl;
    }while(infile.is_open());
    
    return 0;
}

bool is_white_space(char value){
    return value == '\t' || value == '\n' || value == '\r' || value == '\f' || value == ' ';
}

//gets rid of any trailing and/or leading white space when lines from file are read in
string trim_string(string value){
    auto start = value.begin();
    auto end = value.rbegin();
    int length = value.length();
    while(is_white_space(*start)) start++;
    while(is_white_space(*end)) {end++; length--;}
    return string(start,start+length);
}

void fill_nearest_neighbor(ifstream& infile, nearest_neighbor& near_neighor){
    string line;
    bool continueReading = true;
    while(continueReading && getline(infile, line)){
        line = trim_string(line);
        if(!line.compare("#####")){
            continueReading = false;
        }
        else{
            char comma;
            double x,y,z;
            stringstream stream(line);
            stream >> x >> comma >> y >> comma >> z;
            near_neighor.add_point(x,y,z);
        }  
    }

    if(continueReading)
        infile.close();
}