#pragma once

#include <vector>
#include <algorithm>

template<class T>
class heap {
private:
    std::vector<T> heap;
public:
    void push(T value);
    void pop();
    bool empty();
    size_t size();
    T& front();
};

template<class T>
void heap<T>::push(T value){
    heap.push_back(value); 
    std::push_heap(heap.begin(),heap.end());
}

template<class T>
void heap<T>::pop(){
    std::pop_heap(heap.begin(),heap.end()); 
    heap.pop_back();
}

template<class T>
bool heap<T>::empty(){
    return heap.empty();
}

template<class T>
size_t heap<T>::size(){
    return heap.size();
}

template<class T>
T& heap<T>::front(){
    return heap.front();
}