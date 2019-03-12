#include "Search.h"

int main(){

  array<int,10> arr;
  int query;

  cout << "Please input data:" << endl;
  for(int i = 0; i < 10; i++){
    cin >> arr[i];
  }

  int size = arr.size();

  cout << "Enter Query" << endl;
  cin >> query;

  cout << "linear:";
  if(linear(arr, size, query) != -1){
    while(linear(arr, size, query) != 0){
      cout << linear(arr, size, query);
    }
  }
  else{
    while(linear(arr, size, query) != 0){
      cout << linear(arr, size, query);
    }
    cout << "QUERY NOT FOUND" << endl;
  }

  cout << "binary:";
  if(binary(arr, arr[0], arr[size - 1], query) != -1){
    while(binary(arr, arr[0], arr[size - 1], query) != 0){
      cout << binary(arr, arr[0], arr[size - 1], query);
    }
  }
  else{
    while(binary(arr, arr[0], arr[size - 1], query) != 0){
      cout << binary(arr, arr[0], arr[size - 1], query);
    }
    cout << "QUERY NOT FOUND" << endl;
  }

}

int linear(array<int,10> arr, int n, int x){

  int i;
  for (i = 0; i < n; i++){
    if (arr[i] == x){
      return arr[i];
    }
    return arr[i];
  }
  return -1;
}

int binary(array<int,10> arr, int l, int r, int x){

  if(r >= l){
    int mid = l + (r - l)/2;

    if(arr[mid] == x){
      return mid;
    }
    else if(arr[mid] > x){
      return binary(arr, l, mid-1, x);
    }
    else{
      return binary(arr, mid+1, r, x);
    }
  }
  return -1;
}
