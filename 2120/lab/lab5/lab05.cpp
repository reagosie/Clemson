#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include "worldmap.h"


//please add tests to lab05.txt
//there is no need to edit anything in this file
using namespace std;

void fill_world_map(ifstream& infile, worldmap& worldMap);
void find_world_map_path(ifstream& infile, worldmap& worldMap);

int main(){
    ifstream infile("lab05.txt");
    int testnumber = 1;
    do{
        cout << "test" << testnumber++ << endl;
        worldmap worldMap;
        fill_world_map(infile, worldMap);
        find_world_map_path(infile, worldMap);
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

void fill_world_map(ifstream& infile, worldmap& worldMap){
    string line;
    bool continueReading = true;
    while(continueReading && getline(infile, line)){
        line = trim_string(line);
        if(!line.compare("#####")){
            continueReading = false;
        }
        else{
            string city1, city2;
            int distance;
            stringstream stream(line);
            stream >> city1 >> city2 >> distance;
            worldMap.add_pair_to_map(city1, city2, distance);
        }
    }
}

void find_world_map_path(ifstream& infile, worldmap& worldMap){
    string line;
    bool continueReading = true;
    while(continueReading && getline(infile, line)){
        line = trim_string(line);
        if(!line.compare("-----")){
            continueReading = false;
        }
        else{
            string city1, city2;
            stringstream stream(line);
            stream >> city1 >> city2;
            worldMap.find_path(city1, city2);
        }
    }

    if(continueReading)
        infile.close();
}
