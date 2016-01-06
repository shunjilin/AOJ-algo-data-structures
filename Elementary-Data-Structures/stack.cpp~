/**
   Stack
   <Reverse Polish Notation>
   e.g. 
   input:  1 2 + 3 4 - * 
   output: -3
   
   Shunji Lin
   17/12/2015
**/

#include <string>
#include <stack>
#include <iostream>
#include <cstdlib>

int main() {
  
  std::string word; //read expression one by one
  std::stack<int> S; 
  int current1;
  int current2;

  while (std::cin >> word) {
    if (word == "+") {
      current1 = S.top();
      S.pop();
      current2 = S.top();
      S.pop();
      S.push(current1 + current2);
    }
    else if (word == "-") {
      current1 = S.top();
      S.pop();
      current2 = S.top();
      S.pop();
      S.push(current2 - current1); // order matters!
    }
    else if (word == "*") {
      current1 = S.top();
      S.pop();
      current2 = S.top();
      S.pop();
      S.push(current1 * current2);
    } else {
      S.push(atoi(word.c_str())); //convert string to int
    }
  }
  std::cout << S.top() << std::endl;
  
}
