#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <iomanip>
using namespace std;

int main() {
  const char space = ' ';
  const int timeColumnHeader = 4;
  const int cpuColumnHeader = 3;
  const int readyColumnHeader = 21;
  const int column = 4;

  printf("FIFO scheduling results\n\n");
  printf("time   cpu   ready queue (tid/rst)\n");
  printf("----   ---   ---------------------\n");
  for(int i = 0; i < 26; i++){
    cout << right << setw(column) << i << "   A4" << "    --" << endl;
  }
  printf("\n     arrival service completion response wait\n");
  printf("tid   time    time      time      time   time\n");
  printf("---  ------- ------- ---------- -------- ----\n");
  for(int task = 0; task < 3; task++){
    cout << right << " A" << "      3" << "       4" << "        7" << "         4" << "       0" << endl;
  }
  printf("\nservice wait\n");
  printf(" time   time\n");
  printf("------- ----\n");
  for(int t = 0; t < 3; t++){
    cout << right << "   2" << "      4" << endl;
  }

  return 0;
}
