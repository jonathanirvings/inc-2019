#include <bits/stdc++.h>
using namespace std;

const int MAX_N = 1e5;
const int MAX_Q = 1e5;
const int MOD = 1e9 + 7;

struct Progression {
  long long val[4];

  Progression() {
    for (int i = 0; i < 4; i++) val[i] = 0;
  }

  Progression(long long _a, long long _b, long long _c, long long _d) {
    val[0] = _a;
    val[1] = _b;
    val[2] = _c;
    val[3] = _d;
  }

  Progression& operator+=(const Progression& rhs) {
    for (int i = 0; i < 4; i++) {
      // val[i] can be less than 0.
      val[i] = ((val[i] % MOD) + (rhs.val[i] % MOD) + (MOD * 2)) % MOD;
    }
    return *this;
  }
};

struct Node {
  long long sum;
  bool is_lazy;
  Progression p;

  Node() {
    sum = 0;
    is_lazy = 0;
  }
};

int n;
int q;
long long fac[4] = {1, 1, 2, 6};
Node tree[MAX_N * 4];

void update_node(int id, Progression p, long long l, long long r) {
  long long add = 0;
  for (int i = 1; i <= 4; i++) {
    long long x = p.val[i - 1];
    long long y = fac[i - 1];
    for (int j = 1; j < i; j++) {
      x = (x * ((r - l + 1 - j) % MOD)) % MOD;
    }
    add = (add + ((x * y) % MOD) + MOD) % MOD;
  }
  tree[id].sum = (tree[id].sum + add) % MOD;
  if (l != r) {
    long long m = (l + r) / 2;
    // Left.
    tree[id * 2].p += p;
    tree[id * 2].is_lazy = 1;
    // Right.
    long long k = m + 1 - l + 1;
    p.val[1] = (p.val[0] + (((k + 1 - 1) * (p.val[2] - p.val[3])) % MOD)) % MOD;
    p.val[1] = (p.val[1] + ((((((k + 1 - 1) * (k + 1 - 2)) / 2) % MOD) * p.val[3]) % MOD)) % MOD;
    p.val[0] = (p.val[0] + (((k - 1) * (p.val[2] - p.val[3])) % MOD)) % MOD;
    p.val[0] = (p.val[0] + ((((((k - 1) * (k - 2)) / 2) % MOD) * p.val[3]) % MOD)) % MOD;
    p.val[2] = (p.val[2] + (((k - 1) * p.val[3]) % MOD)) % MOD;
    tree[id * 2 + 1].p += p;
    tree[id * 2 + 1].is_lazy = 1;
  }
}

void update(int id, int l, int r, int i, int j, Progression p) {
  if (tree[id].is_lazy) {
    update_node(id, tree[id].p, l, r);
    tree[id].is_lazy = 0;
    tree[id].p = Progression();
  }
  if (r < i || j < l) return;
  if (i <= l && r <= j) {
    update_node(id, p, l, r);
    return;
  }
  long long m = (l + r) / 2;
  // Left.
  update(id * 2, l, m, i, j, p);
  // Right.
  if (i <= m) {
    long long k = m + 1 - max(i, l) + 1;
    p.val[1] = (p.val[0] + (((k + 1 - 1) * (p.val[2] - p.val[3])) % MOD)) % MOD;
    p.val[1] = (p.val[1] + ((((((k + 1 - 1) * (k + 1 - 2)) / 2) % MOD) * p.val[3]) % MOD)) % MOD;
    p.val[0] = (p.val[0] + (((k - 1) * (p.val[2] - p.val[3])) % MOD)) % MOD;
    p.val[0] = (p.val[0] + ((((((k - 1) * (k - 2)) / 2) % MOD) * p.val[3]) % MOD)) % MOD;
    p.val[2] = (p.val[2] + (((k - 1) * p.val[3]) % MOD)) % MOD;
  }
  update(id * 2 + 1, m + 1, r, i, j, p);
  tree[id].sum = (tree[id * 2].sum + tree[id * 2 + 1].sum) % MOD;
}

long long query(int id, int l, int r, int i, int j) {
  if (tree[id].is_lazy) {
    update_node(id, tree[id].p, l, r);
    tree[id].is_lazy = 0;
    tree[id].p = Progression();
  }
  if (r < i || j < l) return 0;
  if (i <= l && r <= j) return tree[id].sum;
  int m = (l + r) / 2;
  long long res = 0;
  // Left.
  res = (res + query(id * 2, l, m, i, j)) % MOD;
  // Right.
  res = (res + query(id * 2 + 1, m + 1, r, i, j)) % MOD;
  return res;
}

long long fast_pow(long long b, long long p) {
  if (p == 0) return 1;
  if (p == 1) return b;
  long long res = fast_pow(b, p / 2);
  res = (res * res) % MOD;
  if (p % 2 == 1) res = (res * b) % MOD;
  return res;
}

void print() {
  for (int i = 0; i < n; i++) {
    printf("%lld%c", query(1, 0, n - 1, i, i), (i == n - 1) ? '\n' : ' ');
  }
}

int main() {
  for (int i = 0; i < 4; i++) {
    fac[i] = fast_pow(fac[i], MOD - 2);
  }
  scanf("%d %d", &n, &q);
  while (q--) {
    int t;
    int l;
    int r;
    scanf("%d %d %d", &t, &l, &r);
    l--;
    r--;
    if (t == 1) {
      long long d = -2;
      long long c = (r - l + 1) + (d * 2);
      long long a = r - l + 1;
      long long b = a + (c - d);
      update(1, 0, n - 1, l, r, Progression(a, b, c, d));
    } else {
      printf("%lld\n", query(1, 0, n - 1, l, r));
    }
    // print();
  }
  return 0;
}
