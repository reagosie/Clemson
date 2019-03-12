/*
 * Name: Reagan Leonard
 * Date Submitted: 9/10/18
 * Lab Section: 003
 * Assignment Name: Lab 3
 */

#pragma once

#include <vector>
using namespace std;

//This class represents a stack of objects
//Do not modify anything in the class interface
template <class T>
class Stack{

private:
    vector<T> data; //stores all of the objects that were added to the stack

public:
    explicit Stack();
    ~Stack();
    bool empty() const;
    const int size() const;
    const T& top() const;
    void push(T value);
    void pop();
    void print() const;
}; //end of class interface (you may modify the code below)

//Implement all of the functions below

//construct an empty stack
template <class T>
Stack<T>::Stack(){

}

//deallocate memory properly to prevent memory leaks
template <class T>
Stack<T>::~Stack(){

}


//Return true if this list is empty
//Otherwise, return false
template <class T>
bool Stack<T>::empty() const{
  if(data.empty()){ //use member function .empty() and if that returns true, return true here
    return true;
  }
  return false;
}

//return a reference to the element at the top of the stack
//(note: this function will only be called when this stack is not empty)
template <class T>
const T& Stack<T>::top() const{
    return data.back(); //use member function that returns reference to the last element and return it
}

//add the value to the top of the stack
template <class T>
void Stack<T>::push(T value){
  data.push_back(value); //use member function that adds element to back of vector and add value
}

//remove the element at the top of the stack
//(note: this function will only be called when the stack is not empty)
template <class T>
void Stack<T>::pop(){
    data.pop_back(); //use member function that removes element from back of vector
}

//return the size of this stack
template <class T>
const int Stack<T>::size() const{
  return data.size(); //use member function that returns size of vector and return that
}

//print out the stack
//do not edit this function
template <class T>
void Stack<T>::print() const{
    Stack<T> stack = *this;
    cout << "(top) ";
    while(!stack.empty()){
        cout << stack.top() << " ";
        stack.pop();
    }
    cout << "(bottom)" << endl;
}
