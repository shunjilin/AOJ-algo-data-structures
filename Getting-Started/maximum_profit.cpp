/**
 Maximum Profit
 <Running List of numbers, find the maximum difference between any 2 numbers,
 a - b, where a has to come after b>

 Shunji Lin
 10/12/2015
 **/

#include <iostream>

using namespace std;

int main() {
  
  int n;
  cin >> n; // number of entries

  int first, second;
  cin >> first >> second;

  int min = (first < second)? first:second; // store minimum of past entries
  int max_diff = second - first; // store current maximum difference

  for (int i = 2; i < n; ++i) {
    int input;
    cin >> input;
    if ((input - min) > max_diff) max_diff = (input - min);
    if (input < min) min = input;
  }

  cout << max_diff << endl;
}

    
      
