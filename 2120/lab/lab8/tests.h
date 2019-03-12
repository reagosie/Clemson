#pragma once

#include <cmath>
#include <vector>
#include <functional>
#include <string>
#include <cfloat>

using namespace std;

struct MinMaxXY{
    double minX;
    double maxX;
    double minY;
    double maxY;
    MinMaxXY(double minX, double maxX, double minY, double maxY):
        minX(minX), maxX(maxX), minY(minY), maxY(maxY){}
};

struct Test{
    string name; //name of the test
    function<double(double,double)> func; //function that will be run
    MinMaxXY minMax;
    bool needYValue;

    Test(string name, function<double(double,double)> func, MinMaxXY minMax, bool needYValue):
        name(name), func(func), minMax(minMax), needYValue(needYValue){} //constructor
};

inline double square(double number){ return pow(number,2); }

double Test1(double x, double y){
    return square(x) + square(x-2);
}

double Test2(double x, double y){
    double numerator = square(sin(square(x)-square(y))) - 0.5;
    double demominator = square(1+0.001*(square(x)+square(y)));
    return 0.5 + numerator/demominator;
}

double Test3(double x, double y){
    return 100*sqrt(abs(y-0.01*square(x))) + 0.01*abs(x+10);
}

double Test4(double x, double y){
    return 2*square(x-3);
}

double Test5(double x, double y){
    return square(1-x) + 100*square(y-x*x);
}

double Test6(double x, double y){
    return square(x+2*y-7) + square(2*x+y-5);
}

/* add tests in vector below
 *
 * all tests need a print out name and a function to run
 *
 * all functions must return a double and take in two
 * doubles as parameters (even if the second parameter
 * is not used)
 */
vector<Test> tests{
    Test("x^2 + (x-2)^2",Test1,MinMaxXY(-100,100,-100,100),false),
    Test("schaffer",Test2,MinMaxXY(-100,100,-100,100),true),
    Test("bukin",Test3,MinMaxXY(-15,-5,-3,3),true),
    Test("mytest1",Test4,MinMaxXY(0,10,0,10),false),
    Test("mytest2",Test5,MinMaxXY(-1.5,1.5,-1.5,1.5),true),
    Test("mytest3",Test6,MinMaxXY(-10,10,-10,10),true),
};
