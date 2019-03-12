/* Name: Reagan Leonard
   Section: 003
   Assignment: Lab 4
   Date: 9/21/18
*/
#pragma once

#include <stack>
#include <iostream>
#include <string>
#include <regex>
#include <vector>
#include <cmath>
#include <locale>
#include <sstream>

using namespace std;

class Calc{
private:
    stack<double> numbers;
    stack<char> operators;

    string regex_match_pattern();
    vector<string> parse_equation(string equation);
    double mult(double num1, double num2);
    double divide(double num1, double num2);
    double add(double num1, double num2);
    double sub(double num1, double num2);
    int precedence(char op);
    int opcall(double num1, double num2, char op);
public:
    double solve_equation(string equation);
};

//returns a string of a regex pattern used to split a formula
string Calc::regex_match_pattern(){
    string leftParentheses = "[(]";
    string rightParentheses = "[)]";
    string operators = "[\\-\\/+*^%]";
    string doubles = "(?:\\d+[.]\\d*|\\d*[.]\\d+|\\d+)";
    return leftParentheses + "|" + rightParentheses + "|" + operators  + "|" + doubles;
}

//takes in an equation in the form of a string, splits it, and returns it in a vector
vector<string> Calc::parse_equation(string equation){
    vector<string> parsedEquation;
    smatch matchingValue;
    regex expression(regex_match_pattern());

    while(regex_search(equation,matchingValue,expression)){
        for(auto match: matchingValue)
            parsedEquation.push_back(match);
        equation = matchingValue.suffix().str();
    }

    return parsedEquation;
}

//takes in an equation, solves it, and returns the answer
double Calc::solve_equation(string equation){
  vector<string> eq = parse_equation(equation);
  double num1 = 0, num2 = 0;
  char op = ' ';
  uint size = eq.size();
  string::size_type np;

  for(uint i = 0; i < size; i++){
    char c = eq[i].at(0);
    if (c == '('){
      operators.push(c);
    }
    else if(c == '^'){
      while(operators.empty() != true && precedence(operators.top()) >= 3 && numbers.size() > 1){
        op = operators.top();
        operators.pop();
        num2 = numbers.top();
        numbers.pop();
        num1 = numbers.top();
        numbers.pop();
        opcall(num1, num2, op);
      }
      operators.push(c);
    }
    else if(c == ')'){
      while(operators.top() != '(' && numbers.size() > 1){
        op = operators.top();
        operators.pop();
        num2 = numbers.top();
        numbers.pop();
        num1 = numbers.top();
        numbers.pop();
        opcall(num1, num2, op);
      }
    }
    else if(c == '*'){
      while(operators.empty() != true && precedence(operators.top()) >= 2 && numbers.size() > 1){
        op = operators.top();
        operators.pop();
        num2 = numbers.top();
        numbers.pop();
        num1 = numbers.top();
        numbers.pop();
        opcall(num1, num2, op);
      }
      operators.push(c);
    }
    else if(c == '/'){
      while(operators.empty() != true && precedence(operators.top()) >= 2 && numbers.size() > 1){
        op = operators.top();
        operators.pop();
        num2 = numbers.top();
        numbers.pop();
        num1 = numbers.top();
        numbers.pop();
        opcall(num1, num2, op);
      }
      operators.push(c);
    }
    else if(c == '%'){
      while(operators.empty() != true && precedence(operators.top()) >= 2 && numbers.size() > 1){
        op = operators.top();
        operators.pop();
        num2 = numbers.top();
        numbers.pop();
        num1 = numbers.top();
        numbers.pop();
        opcall(num1, num2, op);
      }
      operators.push(c);
    }
    else if(c == '+'){
      while(operators.empty() != true && precedence(operators.top()) >= 1 && numbers.size() > 1){
        op = operators.top();
        operators.pop();
        num2 = numbers.top();
        numbers.pop();
        num1 = numbers.top();
        numbers.pop();
        opcall(num1, num2, op);
      }
      operators.push(c);
    }
    else if(c == '-'){
      while(operators.empty() != true && precedence(operators.top()) >= 1 && numbers.size() > 1){
        op = operators.top();
        operators.pop();
        num2 = numbers.top();
        numbers.pop();
        num1 = numbers.top();
        numbers.pop();
        opcall(num1, num2, op);
      }
      operators.push(c);
    }
    else{
        double num = stod(eq[i], &np);
        numbers.push(num);
    }

  }

  while(!operators.empty() && numbers.size() > 1){
    op = operators.top();
    operators.pop();
    num2 = numbers.top();
    numbers.pop();
    num1 = numbers.top();
    numbers.pop();
    switch(op){
      case '*': numbers.push(mult(num1, num2));
                break;
      case '/': numbers.push(divide(num1, num2));
                break;
      case '+': numbers.push(add(num1, num2));
                break;
      case '-': numbers.push(sub(num1, num2));
                break;
      case '^': numbers.push(pow(num1, num2));
                break;
      case '%': numbers.push(fmod(num1, num2));
                break;
    }
    return numbers.top();
  }

  return numbers.top();
}

//function to determine order of operations
int Calc::precedence(char op){
  if(op == '^'){
    return 3;
  }
  else if(op == '*' || op == '/' || op == '%'){
    return 2;
  }
  else if(op == '+' || op == '-'){
    return 1;
  }
  else{
    return 0;
  }
}

//helper function to call operand functions (actually perform operations)
int Calc::opcall(double num1, double num2, char op){
  switch(op){
    case '*': numbers.push(mult(num1, num2));
              break;
    case '/': numbers.push(divide(num1, num2));
              break;
    case '+': numbers.push(add(num1, num2));
              break;
    case '-': numbers.push(sub(num1, num2));
              break;
    case '^': numbers.push(pow(num1, num2));
              break;
    case '%': numbers.push(fmod(num1, num2));
              break;
  }
  return 0;
}

//helper function to multiply two previous numbers together
double Calc::mult(double num1, double num2){
  return num1 * num2;
}

//helper function to divide two previous numbers
double Calc::divide(double num1, double num2){
  return num1 / num2;
}

//helper function to add two previous numbers together
double Calc::add(double num1, double num2){
  return num1 + num2;
}

//helper function to subtract two previous numbers
double Calc::sub(double num1, double num2){
  return num1 - num2;
}
