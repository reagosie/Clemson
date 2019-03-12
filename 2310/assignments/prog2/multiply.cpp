/*Reagan Leonard (rpl)
  CPSC 2310-002
  Programming Assignment 2
  3-13-19
  Description: This program performs multiplication on 2 integers given by the user and shows the steps.*/
#include "multiply.h"

  int main(){
    multiply mult;
    int multiplicand = 0;
    int multiplier = 0;
    cout << "multiplicand: ";
    cin >> multiplicand;
    if(multiplicand < 0 || multiplicand > 255){
      cout << "Error: Multiplicand must be between 0 and 255.";
      exit(0);
    }
    cout << "multiplier: ";
    cin >> multiplier;
    if(multiplier < 0 || multiplier > 255){
      cout << "Error: Multiplier must be between 0 and 255.";
      exit(0);
    }
    cout << endl << endl;

    //PSEUDO-REGISTERS
    int c = 0; //set to 0
    int acc = 0; //set to 0
    int mq = multiplier; //print in decimal and binary
    int mdr = multiplicand; //print in decimal and binary
    int sum = acc; //placeholding value

    cout << "c and acc set to 0" << endl;
    cout << "mq set to multiplier    =  " << multiplier << " decimal and ";
    mult.prt_bin(mq,8);
    cout << " binary" << endl;
    cout << "mdr set to multiplicand =  " << multiplicand << " decimal and ";
    mult.prt_bin(mdr,8);
    cout << " binary" << endl;
    cout << "---------------------------------------------------" << endl;

    int accLoop = acc;
    int mqLoop = mq;
    for(int i = 0; i < 8; i++){
      cout << "step " << i+1 << ":   ";
      mult.prt_bin(c,1);
      cout << " ";
      mult.prt_bin(accLoop,8);
      cout << " ";
      mult.prt_bin(mqLoop,8);
      cout << endl;


      //check lsb; if(lsb = 1), need to call add()
      if(mult.lsb(mqLoop) == 1){
        //sum = mult.add(acc, mdr);
        sum = (accLoop | mqLoop) + (mdr << 8);
        //mult.prt_bin(sum,8);
        cout << "        +   ";
        mult.prt_bin(mdr,8);
        cout << "        ^ add based on lsb=1" << endl;
      }
      else{
        cout << "        +   ";
        mult.prt_bin(0,8);
        cout << "        ^ no add based on lsb=0" << endl;
      }

      cout << "          ----------" << endl;
      cout << "          " << c << " ";
      accLoop = (sum & (0xFF00)) >> 8;
      mult.prt_bin(accLoop,8);
      cout << " ";
      mqLoop = (sum & (0xFF));
      mult.prt_bin(mqLoop, 8);
      cout << endl;

      //need to call shift()
      cout << "       >>                     shift right" << endl;
      cout << "          ";
      sum = mult.shift(sum, mqLoop);
      mult.prt_bin(c,1);
      cout << " ";
      accLoop = (sum & (0xFF00)) >> 8;
      mult.prt_bin(accLoop, 8);
      cout << " ";
      mqLoop = (sum & (0xFF));
      mult.prt_bin(mqLoop, 8);

      // cout << " ";
      // mult.prt_bin(sum,8);
      // cout << " ";
      // mult.prt_bin(mq,8);
      cout << endl;

      cout << "---------------------------------------------------" << endl;
    }
    cout << "check:                  binary   decimal" << endl;
    cout << "                     ";
    mult.prt_bin(multiplicand,8);
    cout << "        " << multiplicand << endl;
    cout << "           x         ";
    mult.prt_bin(multiplier,8);
    cout << "  x     " << multiplier << endl;
    cout << "             ----------------    ------" << endl;
    cout << "             ";

    int result = multiplier * multiplicand;
    mult.prt_bin(result,16);
    cout << "      " << result << endl;

    return 0;
  }
