/*
 * Name: Reagan Leonard
 * Date Submitted: 9/10/18
 * Lab Section: 003
 * Assignment Name: Lab 3
 */

#pragma once

#include <list>
using namespace std;

//This class represents a queue of objects
//Do not modify anything in the class interface
template <class T>
class Queue{

private:
    list<T> data; //stores all of the objects that were added to the queue

public:
    explicit Queue();
    ~Queue();
    bool empty() const;
    const int size() const;
    const T& front() const;
    void push(T value);
    void pop();
    void enqueue(T value);
    void dequeue();
    void print() const;
}; //end of class interface (you may modify the code below)

//Implement all of the functions below

//construct an empty queue
template <class T>
Queue<T>::Queue(){

}

//deallocate memory properly to prevent memory leaks
template <class T>
Queue<T>::~Queue(){

}

//return true if the queue is empty; otherwise, return false
template <class T>
bool Queue<T>::empty() const{
  if(data.empty()){ //use member function that returns true if empty and return that if it is true; otherwise, return false
    return true;
  }
  return false;
}

//return the size of the queue
template <class T>
const int Queue<T>::size() const{
  return data.size(); //use member function that returns size of list and return that
}

//return a reference to the element at the front of the queue
//(note: this function will only be called when the queue is not empty)
template <class T>
const T& Queue<T>::front() const{
  return data.front(); //use member function that returns reference to first element in list and return that
}

//add the value to the back of the queue
template <class T>
void Queue<T>::push(T value){
  data.push_back(value); //use member function that adds element to back of list and use it to add 'value'
}

//remove the element at the front of the queue
//(note: this function will only be called when the queue is not empty)
template <class T>
void Queue<T>::pop(){
  data.pop_front(); //use member function that removes element from from of list and use it to remove first element
}

//same functionality as push
//(note: in many algorithm textbooks, enqueue and push are synonymous operations for queues)
template <class T>
void Queue<T>::enqueue(T value){
  data.push_back(value); //same function as push function
}

//same functionality as pop
//(note: in many algorithm textbooks, dequeue and pop are synonymous operations for queues)
template <class T>
void Queue<T>::dequeue(){
  data.pop_front(); //same function as pop function
}

//print out the queue
//do not edit this function
template <class T>
void Queue<T>::print() const{
    Queue<T> queue = *this;
    cout << "(front) ";
    while(!queue.empty()){
        cout << queue.front() << " ";
        queue.pop();
    }
    cout << "(back)" << endl;
}
