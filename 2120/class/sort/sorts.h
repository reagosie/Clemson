#include <iostream>
#include <stdio.h>
#include <string.h>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
using namespace std;

int main();
vector<double> insertSort(vector<double>& input);
vector<double> selectSort(vector<double>& input);
vector<double> bubbleSort(vector<double>& input);
vector<double> quickSort(vector<double>& input, int left, int right);
int pickPivot(vector<double>& input, int left, int right);
int randPivot(vector<double>& input, int left, int right);
void print(vector<double>& input);
