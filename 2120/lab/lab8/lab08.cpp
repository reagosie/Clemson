#include "tests.h"
#include "optimizers.h"
#include <iostream>
#include <type_traits>

//please add tests to tests.h
//there is no need to edit anything in this file
using namespace std;

int main(){
    for(auto test: tests){
        cout << test.name << endl;
        optimizers optimizer(test.func,test.minMax.minX,test.minMax.maxX,test.minMax.minY,test.minMax.maxY);
        optimizers::minimum_value value = optimizer.greedy();
        cout << "Greedy: min = " << value.value << ", x = " << value.x;
        if(test.needYValue)
            cout << ", y = " << value.y;
        cout << endl;
        value = optimizer.simulated_annealing();
        cout << "SA: min = " << value.value << ", x = " << value.x;
        if(test.needYValue)
            cout << ", y = " << value.y;
        cout << endl << endl;
    }

    return 0;
}
