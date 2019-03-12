#include "sorts.h"

int main(){

  string typesort; //declare string that will identify which sort the user wants
  vector<double> input; //declare vector that will store number inputs

  cout << "Please input data:" << endl;
  //take in string
  cin >> typesort;

  //read in list of numbers
  string numbers;
  double num; //variable for individual number

  getline(cin, numbers); //read in numbers all at once
  stringstream ss(numbers);

  while(ss >> num){
    input.push_back(num); //while there are still numbers to read in,
  }                       //insert them one at a time onto the back of input

  //if string is "this", call this sort
  //do if statements for all types of sort
  if(typesort == "insertion"){
    insertSort(input);
    print(input);
  }
  else if(typesort == "selection"){
    selectSort(input);
    print(input);
  }
  else if(typesort == "bubble"){
    bubbleSort(input);
    print(input);
  }
  else if(typesort == "quick"){
    quickSort(input, 0, input.size() - 1);
    print(input);
  }
  else{ //if typesort does not match any of these strings, print error message
    cout << "Invalid input! Please try again." << endl;
  }

  return 0;
}

vector<double> insertSort(vector<double>& input){ //insertion sort function that takes in reference to input
  int i, j;
  double current; //key will represent the element we are evaluating
  int size = input.size();

    for (i = 1; i < size; i++){
      current = input[i];
      j = i - 1;
      while (j >= 0 && input[j] > current){ //if an element is greater than current, move it to the right
        input[j+1] = input[j];
        j = j - 1;
      }
      input[j+1] = current;
    }
  return input;
}

vector<double> selectSort(vector<double>& input){ //selection sort functionthat takes in reference to input
  int size = input.size();

  for(int i = 0; i < size - 1; i++){
    int min = i; //start with the first element as the min
    for(int j = i + 1; j < size; j++){
      if(input[j] < input[min]){ //if we find a smaller element, that's the new min
        min = j;
      }
    }
    if(min != i){
      swap(input[i], input[min]); //swap min with the first element, and then as i progresses through the vector
    }                              //the min elements will go in their correct place
  }
  return input;
}

vector<double> bubbleSort(vector<double>& input){ //bubble sort function that takes in reference to input
  int size = input.size();

  for(int i = 0; i < size - 1; i++){
    for(int j = 0; j < size - i - 1; j++){
      if(input[j] > input[j+1]){ //compare element with the one next to it
        swap(input[j], input[j+1]); //swap if bigger
      }
    }
  }
  return input;
}

vector<double> quickSort(vector<double>& input, int left, int right){ //quick sort function that takes in reference to input
                                                                      //plus an int that represents the front and back of input
  if(left < right){
    int pivot = randPivot(input, left, right); //make pivot whatever random pivot function gives us
    quickSort(input, left, pivot - 1); //call quicksort on left
    quickSort(input, pivot + 1, right); //call quicksort on right
  }
  return input;
}

int pickPivot(vector<double>& input, int left, int right){ //function to pick pivot point for quick sort
  int pivot = input[right];
  int i = (left - 1);  //i is the index of the smaller element

  for (int j = left; j <= right - 1; j++){
    if (input[j] <= pivot){ //if current element is less than or equal to our pivot
      i++; //increment index of smaller element
      swap(input[i], input[j]); //swap
    }
  }
  swap(input[i + 1], input[right]);
  return (i + 1); //return pivot
}

int randPivot(vector<double>& input, int left, int right){ //function to help pickPivot pick a random pivot point
  srand(time(NULL));
  int pivot = left + rand() % (right - left); //arithmetic to randomize pivot

  swap(input[pivot], input[right]);
  return pickPivot(input, left, right);
}

void print(vector<double>& input){ //function to print out sorted vector of elements
  for(uint i = 0; i < input.size(); i++){
    cout << input[i] << " ";
  }
  cout << endl;
}
