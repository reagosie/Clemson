#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <time.h>

using namespace std;

struct node{
  int value = 0;
  int level = 0;
  node* rightPoint;
  node() : value(0), level(0), rightPoint(NULL){};
  node(int inValue, int inLevel, node* inRightPoint) : value(inValue), level(inLevel), rightPoint(inRightPoint){};
};

struct level{
  //vector<node> nodeNum(1000);
  node nodeNum[1000];
  //level(node inNodeNum[1000]) : nodeNum(inNodeNum[1000]){};
};

struct skipList{
  //vector<level> levely(20);
  level levely[20];
  //skipList(level inLevely[20]) : level(inLevely[20]){};

  void insert(int value, int level){
    int i = 0;
    while(levely[0].nodeNum[i].value < value && levely[0].nodeNum[i].value > 0){
      i++;
    }
    //node nodeT(value, level, NULL);
    for(int m = 0; m < 20; m++){
      for(int k = 1000-1; k >= i; k--){
        levely[m].nodeNum[k].value = levely[m].nodeNum[k-1].value;
      }
    }

    levely[0].nodeNum[i].value = value;
    //levely[0].nodeNum[i+1].level = level;

    for(int j = 1; j < level; j++){
      levely[j].nodeNum[i].value = value;
    }
  }
};

bool coin_flip(){
  bool heads = false;
  int randNum;
  randNum = 1 + rand() % 2;
  if(randNum % 2 == 0){
    heads = true;
  }
  return heads;
}

int generate_level() {
    int n = 0;
    while (coin_flip()){
        n++;
    }

    return n;
}

// int * random(int start, int end){
//   int randArray[end];
//   srand (time(NULL));
//   for(int i = 0; i < end; i++){
//     randArray[i] = rand();
//   }
//   return randArray;
// }

int main(){
  skipList skippy;
  int value = 0;
  int limit = 0;
  char cont = ' ';

  //while(cont != 'N' && cont != 'n'){
  //   cout << "Enter the numbers to insert: " << endl;
  //   cin >> value;
  //   skippy.insert(value, generate_level());
  //   cout << "Continue? Y or N" << endl;
  //   cin >> cont;
   //}

  // for(int k = value; k > 0; k--){
  //   skippy.insert(k, generate_level());
  // }

  srand (time(NULL));
  cout << "Enter number of values: " << endl;
  cin >> limit;
  //int randArray[limit] = random(1, limit);
  for (int n = 0; n < limit; n++){
    skippy.insert(1 + (rand() % 100), generate_level());
  }

  // cout << "SkipList level 3 = ";
  // for(int j = 0; j < limit; j++){
  //   cout << skippy.levely[2].nodeNum[j].value << ", ";
  // }
  // cout << endl;
  //
  // cout << "SkipList level 2 = ";
  // for(int j = 0; j < limit; j++){
  //   cout << skippy.levely[1].nodeNum[j].value << ", ";
  // }
  // cout << endl;
  //
  // cout << "SkipList level 1 = ";
  // for(int i = 0; i < limit; i++){
  //   cout << skippy.levely[0].nodeNum[i].value << ", ";
  // }

  for(int l = 0; l < 15; l++){
    cout << "SkipList level " << l+1 << " = ";
    for(int i = 0; i < limit; i++){
      cout << skippy.levely[l].nodeNum[i].value << ", ";
    }
    cout << endl;
  }

  cout << endl;
  return 0;
}
