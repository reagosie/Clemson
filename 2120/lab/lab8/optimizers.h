/*
 * Name: Reagan Leonard
 * Date Submitted: 10/19/18
 * Lab Section: 002
 * Assignment Name: Lab 8
 */

#pragma once

#include <functional>
#include <cfloat>
#include <random>
#include <ctime>
#include <chrono>
#include <cmath>
#include <limits>
#include <iostream>

using namespace std;

class optimizers{
public:
    struct minimum_value{
        double value;
        double x;
        double y;
        minimum_value(double value, double x, double y):
            value(value), x(x), y(y){}
    };

    optimizers(function<double(double,double)> func,double xMin, double xMax, double yMin, double yMax);
    minimum_value greedy();
    minimum_value simulated_annealing();
private:
    function<double(double,double)> func;
    double xMin, xMax, yMin, yMax;
    double round_number(double number);
    double random_double(double min, double max);
    double get_temperature(double time);
    minimum_value lowestPoint(minimum_value currentValue);
};

optimizers::optimizers(function<double(double,double)> func, double xMin, double xMax, double yMin, double yMax):
    func(func),xMin(xMin),xMax(xMax),yMin(yMin),yMax(yMax)
{

}

//find the lowest point in a graph using a greedy algorithm
optimizers::minimum_value optimizers::greedy(){
  int maxLoopCount = 3000; //maximum number of times we want our for loop to run
  minimum_value globalMinimum(DBL_MAX, DBL_MAX, DBL_MAX); //set global minimum really high

  for(int i = 0; i < maxLoopCount; i++){ //for loop for how many times we want to attempt to find the min
    double x = random_double(xMin, xMax); //pick a random x and y
    double y = random_double(yMin, yMax);
    double value = func(x, y); //declare value using x and y
    minimum_value currentValue(value, x, y); //construct a minimum_value to represent our current value

    minimum_value localMinimum = lowestPoint(currentValue); //call lowestPoint function to calculate minimum

    if(localMinimum.value < globalMinimum.value){ //if what lowestPoint returns is smaller than globalMinimum, update globalMinimum
      globalMinimum = localMinimum;
    }
  }
  globalMinimum.value = round_number(globalMinimum.value); //round all parts of globalMinimum
  globalMinimum.x = round_number(globalMinimum.x);
  globalMinimum.y = round_number(globalMinimum.y);
  return globalMinimum;
}

//find the lowest point in a graph using a simulated annealing algorithm
optimizers::minimum_value optimizers::simulated_annealing(){
  double x = random_double(xMin, xMax); //pick a random x and y
  double y = random_double(yMin, yMax);
  double value = func(x, y); //declare value using x and y
  minimum_value currentLocation(value, x, y); //construct a minimum_value to represent our current location
  minimum_value bestValue = currentLocation; //construct a minimum_value to represent our best value
  double time = 0; //set time to 0
  double P; //probability variable

  while(get_temperature(time) > 0){ //while temp > 0
    double neighborX, neighborY;
    do {
      neighborX = round_number(random_double(-2,2) + currentLocation.x); // pick a random neighbor of currentLocation
      neighborY = round_number(random_double(-2,2) + currentLocation.y);
    } while(neighborX <= xMin && neighborX >= xMax && neighborY <= yMin && neighborY >= yMax);

    minimum_value neighbor(func(neighborX, neighborY), neighborX, neighborY);

    if(neighbor.value < bestValue.value){ // update bestValue if the new value is smaller than bestValue
      bestValue = neighbor;
    }

    P = exp((currentLocation.value-neighbor.value)/get_temperature(time)); //initialize P with temperature function
    if(neighbor.value < currentLocation.value || P > random_double(0,1)){ // update currentLocation if P > rand(0,1)
      currentLocation = neighbor;
    }

    time += 0.01; // increment time by 0.01
  }
  return lowestPoint(bestValue);
}


//this is a function that returns a temperature based on how much time has gone by,
//the temperature gradually approches 0 over time
double optimizers::get_temperature(double time){
    static double initialTemp = 100;
    static double airTemp = 0;
    static double rate = 0.025;
    static double constValue = initialTemp - airTemp;
    double s = constValue * exp(-1*time*rate);
    double temperature = s + airTemp;
    return temperature;
}

//rounds a number to two decimal places
double optimizers::round_number(double number){
    static double scale = 0.01;
    return floor(number/scale + 0.5) * scale;
}

//returns a random double between the two values (inclusive)
double optimizers::random_double(double min, double max){
    static unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    static default_random_engine generator(seed);
    uniform_real_distribution<double> distribution(min,max);
    return round_number(distribution(generator));
}

optimizers::minimum_value optimizers::lowestPoint(minimum_value currentValue){
  minimum_value localMinimum = currentValue; //set localMinimum equal to currentValue at first
  double stepSize = 0.01; //initialize stepSize

  while(true){
    for(double i = -stepSize; i <= stepSize; i+=stepSize){ //nested for loops to check from -stepSize to stepSize
      for(double j = -stepSize; j <= stepSize; j+=stepSize){
        double x = round_number(currentValue.x+i);
        double y = round_number(currentValue.y+j);
        if(x >= xMin && x <= xMax && y >= yMin && y <= yMax){ //check if x and y are in the correct range
          minimum_value newMin(func(x, y), x, y);
          if(newMin.value < currentValue.value){ //update currentValue if new value is less than currentValue
            currentValue = newMin;
          }
        }
      }
    }
    if(currentValue.value < localMinimum.value){ //update localMinimum if currentValue is less than localMinimum
      localMinimum = currentValue;
    }
    else if(localMinimum.value == currentValue.value){ //break if they're equal
      break;
    }
  }

  localMinimum.value = round_number(localMinimum.value); //round every value in localMinimum
  // cout << "value = " << localMinimum.value << endl;
  localMinimum.x = round_number(localMinimum.x);
  // cout << "x = " << localMinimum.x << endl;
  localMinimum.y = round_number(localMinimum.y);
  // cout << "y = " << localMinimum.y << endl;
  return localMinimum;
}
