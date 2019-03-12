/*Reagan Leonard
  CPSC 2121-002
  Lab10
  11/09/18
*/

#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>

using namespace std;

class permute{
public:
  vector<string> numbers;
  //unordered_set<string> set;
  vector<string> tempvec;
  string* array;
  int indicator;
  permute(vector<string> numbers);
  void print_permutation();
private:
  int randIndex(int range);
};

permute::permute(vector<string> numbers){
  this->numbers = numbers;
  this->indicator = 1;
  for(uint i = 0; i < numbers.size(); i++){
    tempvec.push_back(numbers[i]);
  }
  this->array = new string[numbers.size()];
  for(uint j = 0; j < numbers.size(); j++){
    array[j] = numbers[j];
  }
}

void permute::print_permutation(){
  if(indicator == 1){
    for(uint i = 0; i < numbers.size(); i++){
      int d = randIndex(numbers.size());
      string temp;
      temp = numbers[d];
      numbers[d] = numbers[i];
      numbers[i] = temp;
    }

    for(uint j = 0; j < numbers.size(); j++){
      cout << numbers[j] << " ";
    }
    indicator++;
  }

  else if(indicator == 2){
    for(int range = numbers.size(); range > 1; range--){
      int d = randIndex(range);
      string temp;
      temp = tempvec[d];
      tempvec[d] = tempvec[0];
      tempvec[0] = temp;
    }

    for(uint j = 0; j < numbers.size(); j++){
      cout << tempvec[j] << " ";
    }
    indicator++;
  }

  else if(indicator == 3){
    for(int range = numbers.size(); range > 1; range--){
      int d = randIndex(range);
      string temp;
      temp = array[d];
      array[d] = array[range-1];
      array[range-1] = temp;
    }

    for(uint j = 0; j < numbers.size(); j++){
      cout << array[j] << " ";
    }
  }
}

int permute::randIndex(int range){
  srand(time(NULL));
  return rand()%range;
}
