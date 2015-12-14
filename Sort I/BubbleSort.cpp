/**
 Bubble Sort
 <Print # of swap operations>

 Shunji Lin
 10/12/2015
 **/

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
  int n; // number of entries
  cin >> n;

  vector<int> sort_vec;

  for (int i = 0; i < n; i++) {
    int entry;
    cin >> entry;
    sort_vec.push_back(entry);
  }
  

  
  //print vector
  for (int i = 0; i < n; i++) {
    cout << sort_vec[i];
    if (i < n - 1) cout << " ";
  }
  cout << endl;

  cout << n_swaps << endl;
}
	
