/*
 * Name: Reagan Leonard
 * Date Submitted: 9/7/18
 * Lab Section: 003
 * Assignment Name: Lab 2
 */

#ifndef LIST_H
#define LIST_H

#include <iostream>
#include <string>
#include "Node.h"
using namespace std;

//This class represents a linked list of node objects
//that are sorted in descending order.
//Do not modify anything in the class interface
template <class T>
class List{

 private:
  Node<T> * start; //pointer to the first node in this list
  Node<T> * end;   //pointer to the last node in this list
  int mySize;  //size (or length) of this list

 public:
  List();
  ~List();
  int size();
  bool empty();
  void insert(T);
  bool contains(T);
  //Print the name and this list's size and values to stdout
  //This function is already implemented (no need to change it)
  void print(string name){
    cout << name << ": ";
    cout << "size = " << size();
    cout << ", values = ";
    Node<T> * iterator = start;
    while(iterator != NULL){
      cout << iterator->value << ' ';
      iterator = iterator->next;
    }
    cout << endl;
  }

}; //end of class interface (you may modify the code below)

//Implement all of the functions below

//Construct an empty list by initializig this list's instance variables
template <class T>
List<T>::List(){
  start = NULL;
  mySize = 0;
  end = NULL;

}

//Destroy all nodes in this list to prevent memory leaks
template <class T>
List<T>::~List(){
  while(start != NULL){
    Node<T> * iterator = start;
    start = start->next;
    delete iterator;
  }

}

//Return the size of this list
template <class T>
int List<T>::size(){
  return mySize;

}

//Return true if this list is empty
//Otherwise, return false
template <class T>
bool List<T>::empty(){
  if(start == NULL){
    return true;
  }
  else{
    return false;
  }

}

//Create a new node with value, and insert that new node
//into this list in its correct position such that
//the values of the nodes in this list are sorted in descending order
template <class T>
void List<T>::insert(T value){
  Node<T> * newNode = new Node<T>(value);
  //newNode->value = value;

  if(empty() == true){
    start = newNode;
    end = newNode;
  }
  else if(empty() == false){
    if(newNode->value > start->value){
      newNode->next = start;
      start = newNode;
    }
    else if(newNode->value < start->value){
      end->next = newNode;
      end = newNode;
    }	
    else{
      for(Node<T> * iterator = start; iterator->next != end; iterator = iterator->next){
        if(iterator->value >  newNode->value && newNode->value > iterator->next->value){
          newNode->next = iterator->next;
	  iterator->next = newNode;
        }
      }
    }
  }
  mySize++;
}

//Return true if this list contains a node whose value is equal to the key
//Otherwise, return false
template <class T>
bool List<T>::contains(T key){
  for(Node<T> * iterator = start; iterator->next != end; iterator = iterator->next){
    if(iterator->value == key){
      return true;
    }
  }
  return false;

}

#endif
