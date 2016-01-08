/**
 <Exhaustive Search>

 http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ALDS1_5_A
 
 can sum be made using given inputs?

 Recursive solution

 As problem involes multiple queries,
 can do better by dynamic programming (not implemented)
 
 Shunji Lin
 08/01/2016
 **/

#include <cstdio>

bool solved;
int inputs[20];

// recursive check if sum can be made by picking / not picking
// each index of inputs array
void check(int index, int sum, int max_index) {
  if (solved) return; // early termination
  int new_sum = sum - inputs[index]; // pick current index
  if (new_sum == 0) { // solved
    solved = true;
    return;
  }
  if (index < max_index) {
    if (new_sum > 0) check(index + 1, new_sum, max_index); // pick cur index if does not exceed sum
    check(index + 1, sum, max_index); // don't pick cur index
  }
}

int main() {
  int n; // number of inputs
  scanf("%d", &n);
  for (int i = 0; i < n; ++i) {
    scanf("%d", &inputs[i]);
  }
  int m; // number of sum queries
  scanf("%d", &m);
  for (int i = 0; i < m; ++i) {
    int sum;
    scanf("%d", &sum);
    solved = false;
    check(0, sum, n-1);
    solved ? printf("yes\n") : printf("no\n");
  }
}
