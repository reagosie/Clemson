/*
 * Name: Reagan Leonard
 * Date Submitted:
 * Lab Section: 003
 * Assignment Name: Lab 5
 */

#pragma once

#include <string>
#include <iostream>
#include <map>
#include <vector>
#include <queue>
#include <set>
#include <stack>

using namespace std;

//This class represents a map of all of the cities
//feel free to add to this class but do not remove anything
class worldmap{
private:
    void breadth_first_search(string city1, string city2);
    void depth_first_search(string city1, string city2);
    map <string, vector <pair <string, int> > > cities;
    set<string> visited;
    bool found = false;
    int d = 0;
public:
    void add_pair_to_map(string city1, string city2, int distance);
    void find_path(string city1, string city2);
  };

//Implement all of the functions below

//takes in two cities that are connected, the distance between them,
//and stores the values
void worldmap::add_pair_to_map(string city1, string city2, int distance){
  cities[city1].push_back(pair<string, int>(city2, distance));
  cities[city2].push_back(pair<string, int>(city1, distance));
}

//finds the path between two cities using breath first search
//prints out cities visited during the search
//as well as the total distance traveled
void worldmap::breadth_first_search(string city1, string city2){
  //use queue
  queue<pair<string, int> > nextcity;
  set<string> visitedcities;
  pair<string, int> current;
  int dist = 0;

  nextcity.push(pair<string, int>(city1, 0));

  while((current = nextcity.front()).first != city2){
    cout << current.first << ", ";
    dist += current.second;
    visitedcities.insert(current.first);

    for(auto childcity : cities[current.first]){
      if(!visitedcities.count(childcity.first)){
        nextcity.push(childcity);
      }
    }
    nextcity.pop();
  }
  cout << current.first << "; ";
  dist += current.second;
  cout << "distance traveled: " << dist << endl;
}

//finds the path between two cities using depth first search
//prints out cities visited during the search
//as well as the total distance traveled
void worldmap::depth_first_search(string city1, string city2){
  vector<pair<string, int> > nextcity = cities[city1];
  visited.insert(city1);
  cout << city1 << ", ";

  for(auto i = nextcity.begin(); i != nextcity.end() && !found; i++){
    //cout << i->first << " in for loop.  ";
    if(i->first == city2){
      found = true;
      cout << i->first << "; ";
      d += i->second;
      cout << "distance traveled: " << d << endl;
    }
    else if(!visited.count(i->first)){
      d += i->second;
      depth_first_search(i->first, city2);
    }
  }
}

//this function finds the path between two cities
//using breath_first_search first and then depth_first_search
void worldmap::find_path(string city1, string city2){
  cout << "BFS: ";
  breadth_first_search(city1, city2);
  cout << "DFS: ";
  visited.clear();
  found = false;
  d = 0;
  depth_first_search(city1, city2);
}
