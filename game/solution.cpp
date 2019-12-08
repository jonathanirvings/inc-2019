#include <bits/stdc++.h>

int main() {
  int N, K;
  scanf("%d %d", &N, &K);
  int ans = 0;
  while (N--) {
    scanf("%d", &K);
    ans ^= (K & 1);
  }
  puts(ans ? "John" : "Preston");
}
