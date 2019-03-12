#include <stdio.h>
#include <iostream>

using namespace std;

int main(){
  skipList skippy;
  int value = 0;
  char cont = ' ';

  while(cont != 'N' && cont != 'n'){
    cout << "Enter the number to insert: " << endl;
    cin >> value;
    skippy.insert(value, generate_level());
    cout << "Continue? Y or N" << endl;
    cin >> cont;
  }

  return 0;
}
