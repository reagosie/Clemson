#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include "graphalgs.h"

using namespace std;

node fill_graph(graphalgs& graph, ifstream& infile);

int main(){
    ifstream infile("lab09.txt");
    int testNumber = 1;
    do{
        cout << "---Test " << testNumber++ << "---" <<endl;
        graphalgs graph;
        node sourceNode = fill_graph(graph,infile);
        graph.dijkstra(sourceNode);
        graph.kruskal();

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

node fill_graph(graphalgs& graph, ifstream& infile){
    node sourceNode = "";
    string line;

    bool continueReading = true;

    while(continueReading && getline(infile,line)){
        line = trim_string(line);
        stringstream stream(line);
        if(line == "#####"){
            continueReading = false;
            continue;
        }

        node node1,node2;
        int distance;

        getline(stream,node1,',');
        getline(stream,node2,',');
        stream >> distance;

        if(sourceNode == "")
            sourceNode = node1;
        graph.add_edge(node1,node2,distance);
    }

    if(continueReading)
        infile.close();
    return sourceNode;
}