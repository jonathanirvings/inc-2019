#include <bits/stdc++.h>
using namespace std;

int main() {
  int N;
  scanf("%d", &N);

  string S, T;
  cin >> S >> T;

  vector<int> cnt(256, 0);
  for (int i = 0; i < N; ++i) {
    ++cnt[S[i]];
    if (S[i] != T[i]) {
      ++cnt[T[i]];
    }
  }

  printf("%d\n", *max_element(cnt.begin(), cnt.end()));
}
