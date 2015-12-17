/**
 Greatest Common Divisor 
 <Finds the greatest common divisor of two natural numbers a and b> 

 Shunji Lin
 5/12/2015
 **/

#include <iostream>

using namespace std;

int gcd (int a, int b) {
  // a >= b
  return (b == 0) ? a : gcd(b, a%b);
}

int main() {
  int a, b;
  cin >> a >> b;
   
  (a > b) ? cout << gcd(a,b) : cout << gcd(b,a);
 
  cout << endl;
}

  
  

  
