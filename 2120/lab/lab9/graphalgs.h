/*
 * Name: Reagan Leonard
 * Date Submitted: 11/2/18
 * Lab Section: 002
 * Assignment Name: Lab9
 */

#pragma once

#include <string>
#include <climits>
#include <map>
#include <set>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include "heap.h"

using namespace std;
typedef std::string node;

class edge{

public:
  node node1;
  int weight;
  edge(node node2, int distance);
  bool operator < (const edge &node);
};

bool edge::operator < (const edge &node){
  if(weight > node.weight){
    return true;
  }
  return false;
}

edge::edge(node node2, int distance){
  node1 = node2;
  weight = distance;
}

class kruskaledge{
public:
  node node1;
  node node2;
  int distance;
  kruskaledge(node node1, node node2, int distance);
  bool operator < (const kruskaledge &node);
};

bool kruskaledge::operator < (const kruskaledge &node){
  if(distance < node.distance){
    return true;
  }
  return false;
}

kruskaledge::kruskaledge(node node1, node node2, int distance){
  this->node1 = node1;
  this->node2 = node2;
  this->distance = distance;
}

class graphalgs{
public:
    void add_edge(node node1, node node2, int distance);
    void dijkstra(node firstNode);
    void kruskal();
    bool isCycle(map<node, vector<edge>> kruskal);
    bool isCycleUtil(node v, set<node> &visited, node parent, map<node, vector<edge>> kruskal);
    map <node, vector<edge> > graph;
    vector<kruskaledge> edges;
    int V;
};

//adds two nodes that are directly connected and their distances
//all edges will be added before dijkstra and kruskal are called
void graphalgs::add_edge(node node1, node node2, int distance){
  graph[node1].push_back(edge(node2, distance));
  graph[node2].push_back(edge(node1, distance));
  edges.push_back(kruskaledge(node1, node2, distance));
}

//prints the minimum distance from the source node to the other nodes
void graphalgs::dijkstra(node sourceNode){
  map<node, int> table;
  set<string> visited;
  heap<edge> minHeap;
  minHeap.push(edge(sourceNode, 0));
  table[sourceNode] = 0;

  for(auto i:graph){
    if(i.first != sourceNode){
      table[i.first] = INT_MAX;
    }
  }

  while(visited.size() != graph.size()){
    edge current = minHeap.front();
    minHeap.pop();
    if(visited.count(current.node1) == 0){
      visited.insert(current.node1);
      for(auto neighbor:graph[current.node1]){
        int newdist = current.weight + neighbor.weight;
        if(newdist < table[neighbor.node1]){
          table[neighbor.node1] = newdist;
          minHeap.push(edge(neighbor.node1, newdist));
        }
      }
    }
  }

  cout << "Dijkstra (from " << sourceNode << ")" << endl;
  for(auto t:table){
    if(t.first != sourceNode){
    cout << t.first << ": " << t.second << endl;
    }
  }
}

//prints the edges of the graph that create a minimum spanning tree
void graphalgs::kruskal(){
  //sort all edges
  sort(edges.begin(), edges.end()); //sort edges

  //add edges one at a time (unless they create a loop) until all nodes visited
  vector<kruskaledge> mst; //vector of edges that will be our mst (this is what we are "adding" t0)
  map <node, vector<edge> > kruskal; //make new graph for minimum spanning tree
  int i = 0;
  while(mst.size() < graph.size()-1){
    kruskal[edges[i].node1].push_back(edge(edges[i].node2, edges[i].distance));
    kruskal[edges[i].node2].push_back(edge(edges[i].node1, edges[i].distance));
    if(isCycle(kruskal)){
      kruskal[edges[i].node1].pop_back();
      kruskal[edges[i].node2].pop_back();
    }
    else{mst.push_back(edges[i]);}
    i++;
  }
  //print output
  cout << "Kruskal" << endl;
  int totalweight = 0;
  for(auto t:mst){
    cout << t.node1 << ", " << t.node2 << ", " << t.distance << endl;
    totalweight += t.distance;
  }
  cout << "total weight = " << totalweight << endl;
}

bool graphalgs::isCycle(map<node, vector<edge>> kruskal){
  // Mark all the vertices as not visited and not part of recursion stack
  //V = kruskal.size();
  set<string> visited;

  // Call the recursive helper function to detect cycle in different
  // DFS trees
  for(auto t:kruskal){
    if(visited.count(t.first) == false){
      if (isCycleUtil(t.first, visited, "", kruskal)){
        return true;
      }
    }
  }
  return false;
}

bool graphalgs::isCycleUtil(node v, set<node> &visited, node parent, map<node, vector<edge>> kruskal){
  if(visited.count(v) == false){
  // Mark the current node as visited and part of recursion stack
    visited.insert(v);

    // Recur for all the vertices adjacent to this vertex
    for(auto t:kruskal[v]){
      if(!visited.count(t.node1)){
        if (isCycleUtil(t.node1, visited, v, kruskal))
          return true;
      }
      else if (t.node1 != parent){
        return true;
      }
    }

  }
  return false;
}
