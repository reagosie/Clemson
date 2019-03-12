#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include "permute.h"

using namespace std;

void run_permutation(vector<string>& data);

int main(){
    ifstream infile("lab10.txt");
    string line;
    while(getline(infile,line)){
        vector<string> data;
        stringstream stream(line);
        string value;
        while(getline(stream,value,','))
            data.push_back(value);
        
        run_permutation(data);
    }    
}

void run_permutation(vector<string>& data){
    permute test(data);
    cout << "First:\t\t"; test.print_permutation(); cout << endl;
    cout << "Second:\t\t";  test.print_permutation(); cout << endl;
    cout << "Third:\t\t";   test.print_permutation(); cout << endl;
}