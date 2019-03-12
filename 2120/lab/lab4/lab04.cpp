/* Name: Reagan Leonard
   Section: 003
   Assignment: Lab 4
   Date: 9/21/18
*/
#include <iostream>
#include <string>
#include "Calc.h"
#include <vector>

using namespace std;

int main(int argc, char* argv[]){
    Calc calculator;
    vector<string> equations{"((2+2)-2)*2","(1+2)^2*((3-4)/5)","(4^3)%3","(1+2)","5-2","81/27","5^0","16%7"};
    for(string equation: equations){
        cout << equation << endl;
        cout << calculator.solve_equation(equation) << endl;
    }
    return 0;
}
