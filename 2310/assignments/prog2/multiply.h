#include <stdio.h>
#include <iostream>
#include <math.h>
using namespace std;

class multiply{
private:
  int c = 0;
public:
  void prt_bin( int value, int length );
  int lsb(int decimal);
  int shift(int sum, int mq);
};

void multiply::prt_bin( int value, int length ){
  int i;
  for( i=(length-1); i>=0; i--){
    if((value>>i)&1)
       putchar('1');
    else
       putchar('0');
  }
}

int multiply::lsb(int decimal){
  //make an array to hold binary number (no carry)
  int bin[7];

  int i = 7;
  while(decimal > 0){
    bin[i] = decimal % 2;
    decimal = decimal / 2;
    i--;
  }

  //return the last significant bit (rightmost) which is at the end of the array
  return bin[7];
}

//we want to take in these 3 numbers (pass by reference), change them individually, don't return anything
int multiply::shift(int sum, int mq){
  int result = (sum|mq) >> 1;
  return result;
}
