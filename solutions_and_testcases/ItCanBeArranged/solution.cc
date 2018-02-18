#include <cstring>
#include <iostream>
#include <cstdio>
using namespace std;

int N, M;
int A[100], B[100], S[100], clean[100][100];

int cap[202][202], seen[202];
int flow(int x, int cf, int e) {
  if (x == e) return cf;
  if (seen[x]) return 0;
  seen[x] = true;
  for (int y = 0; y < 2+N+N; y++) if (cap[x][y]) {
    int f = flow(y, min(cf, cap[x][y]), e);
    if (f) {
      cap[x][y] -= f;
      cap[y][x] += f;
      return f;
    }
  }
  return 0;
}

int main() {
 //   freopen("..\\IO\\input.in","r",stdin);
  //  freopen("..\\IO\\outputdk.ans","w",stdout);
  int T, prob=1;
  for (cin >> T; T--;) {
    cin >> N >> M;
    for (int i = 0; i < N; i++) cin >> A[i] >> B[i] >> S[i];
    for (int i = 0; i < N; i++)
    for (int j = 0; j < N; j++)
      cin >> clean[i][j];

    int ret = 0;
    for (int i = 0; i < N; i++)ret += (S[i] + M-1) / M;
    memset(cap, 0, sizeof(cap));
    for (int i = 0; i < N; i++) cap[0][2+i]   = (S[i] + M-1) / M;
    for (int i = 0; i < N; i++) cap[2+N+i][1] = (S[i] + M-1) / M;
    for (int i = 0; i < N; i++)
    for (int j = 0; j < N; j++) if (j != i) {
      if (B[i] + clean[i][j] < A[j]) cap[2+i][2+N+j] = 1000000000;
    }

    for (;;) {
      memset(seen, 0, sizeof(seen));
      int f = flow(0, 1000000000, 1);
      if (!f) break;
      ret -= f;
    }
    cout << "Case " << prob++ << ": " << ret << endl;
  }
  return 0;
}
