#include <iostream>
#include <string>
#include <vector>
using namespace std;

#define MOD 21092013

main() {
  int N, prob=1;
  for (cin >> N; N--;) {
    string S, T;
    cin >> S >> T;

    vector<char> st;
    for (int i = 0; i < S.size(); i++) {
      if (S[i] == 'U') {
        if (st.size()) st.pop_back();
      } else {
        st.push_back(S[i]);
      }
    }

    vector<int> nextL(T.size()), nextR(T.size());
    int nl = T.size(), nr = T.size();
    for (int i = T.size()-1; i >= 0; i--) {
      nextL[i] = nl; nextR[i] = nr;
      if (T[i] == 'L') nl = i;
      if (T[i] == 'R') nr = i;
    }
    vector<int> dp(T.size()+2);
    dp[T.size()] = 1;
    for (int i = T.size()-1; i >= 0; i--) {
      if (T[i] == 'L') {
        dp[i] = 2*dp[i+1] - dp[nextL[i]+1];
      } else if (T[i] == 'R') {
        dp[i] = 2*dp[i+1] - dp[nextR[i]+1];
      } else {
        dp[i] = dp[i+1];
      }
      dp[i] %= MOD;
    }

    int ret = dp[0];
    for (int i = 0; i < T.size() && st.size(); i++) {
      if (T[i] == 'U') {
        ret++;
        if (st.back() == 'L') {
          ret = (ret + dp[nextR[i]+1]) % MOD;
        } else if (st.back() == 'R') {
          ret = (ret + dp[nextL[i]+1]) % MOD;
        }
        st.pop_back();
      }
    }

    cout << "Case " << prob++ << ": " << (ret + MOD) % MOD << endl;
  }
}
