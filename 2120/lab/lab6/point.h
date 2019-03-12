/*Reagan Leonard
  10/5/18
  Section 003
  Lab 6
*/
#pragma once
#include <cmath>
#include <iostream>
using namespace std;

//this is a class for what a point in this data set should consist of
class Point{
public:
  double x, y, z;
  static double distance(Point& a, Point& b);
  Point();
  Point(double x, double y, double z);
  bool operator < (const Point& p) const;
  static void printPoint(Point& a, Point& b);
};

//this function calculates Euclidean distance
double Point::distance(Point& a, Point& b){
  double dist1 = (pow((b.x-a.x), 2));
  double dist2 = (pow((b.y-a.y), 2));
  double dist3 = (pow((b.z-a.z), 2));
  return sqrt((dist1 + dist2 + dist3));
}

//default constructor
Point::Point(){x=0; y=0; z=0;}

//parameter constructor
Point::Point(double x, double y, double z){
  this->x = x;
  this->y = y;
  this->z = z;
}

//overloading the < operator
bool Point::operator < (const Point& p) const{
  if(x != p.x){
    return (x < p.x);
  }
  else if(y != p.y){
    return (y < p.y);
  }
  else if(z != p.z){
    return (z < p.z);
  }
  else{return false;}
}

//prints our output
//prints the two closest points and the distance between them
void Point::printPoint(Point& a, Point& b){
  cout << "Nearest neighbors: " << "(" << a.x << "," << a.y << "," << a.z << ")" << " and " << "(" << b.x << "," << b.y << "," << b.z << ")"<< endl;
  cout << "Distance: " << Point::distance(a, b) << endl;
}
