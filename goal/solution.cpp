#include <bits/stdc++.h>
using namespace std;

const int N = 2e5 + 5;
const int MOD = 1e9 + 7;

int modPow(int base, int po) {
  int ret = 1;
  
  while (po) {
    if (po & 1) ret = 1ll * ret * base % MOD;
    po >>= 1;
    base = 1ll * base * base % MOD;
  }

  return ret;
}

int fact[N];
int inve[N];

void precompute() {
  fact[0] = 1;
  inve[0] = 1;

  for (int i = 1 ; i < N ; i++) {
    fact[i] = 1ll * i * fact[i-1] % MOD;
    inve[i] = modPow(fact[i], MOD-2);
  }
}

int calcWays(vector<int> v) {
  int ret = 1;
  int tot = 0;

  for (int x : v) {
    ret = 1ll * ret * inve[x] % MOD;
    tot += x;
  }

  ret = 1ll * ret * fact[tot] % MOD;
  return ret;
}

int work(int n, int m) {
  int ret = 0;
  int cnt = 0;

  for (int trailingSuper = 0 ; trailingSuper < 3 ; trailingSuper++) {
    for (int superTwice = 0 ; superTwice <= m ; superTwice++) {
      int superOnce = n - 2 * superTwice - trailingSuper;
      int noSuper = m - superTwice - superOnce;

      if (superOnce < 0) continue;
      if (noSuper < 0) continue;

      int ways = calcWays({noSuper, superOnce, superTwice});
      ret = (ret + ways) % MOD;
      cnt++;
    }
  }

  return ret;
}

int main() {
  precompute();

  int n, m;
  cin >> n >> m;

  cout << work(n, m) << endl;
  return 0;
}
