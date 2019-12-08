#include <bits/stdc++.h>
using namespace std;

const int MAX_TOTAL = 60 * 200;
const long long NOT_COMPUTED = -1;

int R, C;
vector<vector<int>> A;
long long K;
vector<vector<vector<long long>>> dp;

long long f(int x, int y, int sum) {
  if (make_pair(x, y) == make_pair(R - 1, C - 1)) {
    return (sum == A[x][y] ? 1 : 0);
  }
  if (x >= R || y >= C || sum < 0) {
    return 0;
  }
  long long &ret = dp[x][y][sum];
  if (ret != NOT_COMPUTED) {
    return ret;
  }
  ret = f(x + 1, y, sum - A[x][y]) + f(x, y + 1, sum - A[x][y]);
  return ret;
}

void backtrack(int x, int y, int sum, long long K) {
  if (make_pair(x, y) == make_pair(R - 1, C - 1)) {
    return;
  }
  long long down = f(x + 1, y, sum - A[x][y]);
  if (K <= down) {
    printf("D");
    backtrack(x + 1, y, sum - A[x][y], K);
  } else {
    printf("R");
    backtrack(x, y + 1, sum - A[x][y], K - down);
  }
}

int main() {
  scanf("%d %d %lld", &R, &C, &K);
  A.resize(R, vector<int>(C));
  for (int i = 0; i < R; ++i) {
    for (int j = 0; j < C; ++j) {
      scanf("%d", &A[i][j]);
    }
  }
  dp.resize(
    R,
    vector<vector<long long>>(C,vector<long long>(MAX_TOTAL + 1, NOT_COMPUTED))
  );
  for (int total = MAX_TOTAL; total >= 0; --total) {
    long long cur = f(0, 0, total);
    if (K <= cur) {
      backtrack(0, 0, total, K);
      break;
    } else {
      K -= cur;
    }
  }
  puts("");
}
