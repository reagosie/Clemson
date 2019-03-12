/*Reagan Leonard
  10/5/18
  Section 003
  Lab 6
*/
#pragma once
#include "point.h"
#include <vector>
#include <algorithm>

using namespace std;

//class with functions to find the closest points
class nearest_neighbor{
public:
    void add_point(double x, double y, double z);
    void find_closest_points();
    pair<Point, Point> divideConquer(int begin, int end);
private:
    vector<Point> points;
};

//function to insert a new point
void nearest_neighbor::add_point(double x, double y, double z){
  Point p(x,y,z);
  points.push_back(p);
}

//function that finds the two points that are the closest
//also calls printPoint to print the closest points and the distance between them
void nearest_neighbor::find_closest_points(){
  sort(points.begin(), points.end());
  pair<Point, Point> closest = divideConquer(0, points.size()-1);
  Point::printPoint(closest.first, closest.second);
}

//this is the function that we call recursively
//this function will return our two closest points
pair<Point, Point> nearest_neighbor::divideConquer(int begin, int end){
  pair<Point, Point> lmin, rmin;
  pair<Point, Point> closest(points[begin], points[end]);
  int size = end - begin;
  int mid = begin+((size)/2);

  if(size == 1){
    return closest;
  }
  else if(size == 2){
    pair<Point, Point> min1(points[begin], points[begin+1]);
    pair<Point, Point> min2(points[begin], points[end]);
    pair<Point, Point> min3(points[begin+1], points[end]);

    if(Point::distance(min1.first, min1.second) < Point::distance(min2.first, min2.second) && Point::distance(min1.first, min1.second) < Point::distance(min3.first, min3.second)){
      return min1;
    }
    else if(Point::distance(min2.first, min2.second) < Point::distance(min3.first, min3.second) && Point::distance(min2.first, min2.second) < Point::distance(min1.first, min1.second)){
      return min2;
    }
    else{return min3;}
  }
  else{
    lmin = divideConquer(begin, mid);
    rmin = divideConquer(mid+1, end);
    if(Point::distance(lmin.first, lmin.second) < Point::distance(rmin.first, rmin.second)){
      /*int redge = mid + (int)Point::distance(lmin.first, lmin.second);
      int ledge = mid - (int)Point::distance(lmin.first, lmin.second);
      for(int i = ledge; i <= redge; i++){
        for(int j = i; j <= redge; j++){
          if(Point::distance(points[i], points[j]) < Point::distance(lmin.first, lmin.second)){
            pair<Point, Point> close(points[i], points[j]);
            return close;
          }
        }
      }*/
      return lmin;
    }
    else{
      /*int redge = mid + (int)Point::distance(rmin.first, rmin.second);
      int ledge = mid - (int)Point::distance(rmin.first, rmin.second);
      for(int i = ledge; i <= redge; i++){
        for(int j = i; j <= redge; j++){
          if(Point::distance(points[i], points[j]) < Point::distance(rmin.first, rmin.second)){
            pair<Point, Point> close(points[i], points[j]);
            return close;
          }
        }
      }*/
      return rmin;
    }
  }
}
