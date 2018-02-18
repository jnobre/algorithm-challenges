#include <iostream>
#include <string>
using namespace std;

// 2^(ITER-3) >= len(S)+1 guarantees we go far enough.
#define ITER 10 

main() {
  string f[ITER+1];
  for (int i = 1; i <= ITER; i++) {
    f[i] = f[i-1] + 'L';
    for (int j = f[i-1].size()-1; j >= 0; j--)
      f[i] += (f[i-1][j] == 'L' ? 'R' : 'L');
  }

  int T, prob=1;
  for (cin >> T; T--;) {
    int N;
    string s;
    cin >> N >> s;
    cout << "Case " << prob++ << ": ";
    cout << (f[min(N, ITER)].find(s) == -1 ? "No" : "Yes") << endl;
  }
}

/*
Here's my solution to I.  Kind of interesting that a really basic approach ends up working.  You should include test cases with the string "LLLRRLRRRLLRLLRRRLLRRLRRRLLRLLRRLLLRRLRRLLLRLLRRRLLRRLRRLLLRLLRRLLLRRLRRRLLRLLRRRLLRRLRRLLLRLLRRLLLR", since it's one of the few length-100 strings that shows up at N=10 and not N=9.  (If a length-100 string doesn't show up at N=10, it never will.)

I'd give it a difficulty of 3 and an interest of 8.
*/