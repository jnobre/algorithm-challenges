#include <bits/stdc++.h>
using namespace std;

vector<int> array_left_rotation(vector<int> a, int n, int k) {
  vector<int> output(n);
  for (int i = 0; i < n; i++) {
     output[i] = a[(i + k) % n];
  }
  return output;
}

int main() {
  int n;
  int k;
  cin >> n >> k;
  vector<int> a(n);
  for (int i = 0; i < n; i++) {
    cin >> a[i];
  }
  vector<int> output = array_left_rotation(a, n, k);
  for (int i = 0; i < n; i++)
    cout << output[i] << " ";

  return 0;
}
