#include <bits/stdc++.h>
using namespace std;

const int N = 1e5 + 5;
const int LOG = 17;

pair<int, int> anc[N][LOG];
int depth[N];
int treeRoot[N];

int cycleCnt;
int cycleVals[2 * N];
int cycleIndex[N];
int stree[2 * 4 * N];

int deg[N];

vector<pair<int, int>> adj[N];
int n, q;

void buildST(int now, int par, int w) {
  depth[now] = depth[par] + 1;
  anc[now][0] = {par, w};

  for (int i = 1; (1 << i) <= depth[now]; i++) {
    int papa = anc[now][i - 1].first;
    int grandpa = anc[papa][i - 1].first;
    int mins = min(anc[now][i - 1].second, anc[papa][i - 1].second);

    anc[now][i] = {grandpa, mins};
  }
}

void dfs(int now, int prv, int root) {
  for (pair<int, int> edge : adj[now]) {
    int nex = edge.first;
    int w = edge.second;

    if (deg[nex] == 2 || nex == prv) continue;

    treeRoot[nex] = root;
    buildST(nex, deg[now] == 2 ? 0 : now, w);

    dfs(nex, now, root);
  }
}

pair<int, int> getLCA(int u, int v) {
  int mins = 1e9;

  if (depth[u] < depth[v]) swap(u, v);
  int diff = depth[u] - depth[v];

  for (int i = 0 ; diff ; i++) {
    if (diff & (1 << i)) {
      diff -= (1 << i);
      mins = min(mins, anc[u][i].second);
      u = anc[u][i].first;
    }
  }

  if (u == v) return {u, mins};

  for (int i = LOG-1 ; i >= 0 ; i--) {
    if ((1 << i) <= depth[u] && anc[u][i].first != anc[v][i].first) {
      mins = min(mins, anc[u][i].second);
      mins = min(mins, anc[v][i].second);
      u = anc[u][i].first;
      v = anc[v][i].first;
    }
  }

  mins = min(mins, anc[u][0].second);
  mins = min(mins, anc[v][0].second);

  return {anc[u][0].first, mins};
}

void buildSegTree(int id, int l, int r) {
  if (l == r) {
    stree[id] = cycleVals[l];
  } else {
    int m = (l + r) / 2;
    int chld = id << 1;

    buildSegTree(chld, l, m);
    buildSegTree(chld + 1, m+1, r);

    stree[id] = min(stree[chld], stree[chld+1]);
  }
}

int querySegTree(int id, int l, int r, int x, int y) {
  if (x <= l && r <= y) {
    return stree[id];
  } else {
    int m = (l + r) / 2;
    int chld = id << 1;

    int ret = 1e9;
    if (x <= m) ret = min(ret, querySegTree(chld, l, m, x, y));
    if (y > m)  ret = min(ret, querySegTree(chld+1, m+1, r, x, y));

    assert(ret > 0);
    return ret;
  }
}

void initDeg() {
  queue<int> q;
  for (int i = 1 ; i <= n ; i++) {
    deg[i] = adj[i].size();
    if (deg[i] == 1) {
      q.push(i);
    }
  }

  while (!q.empty()) {
    int now = q.front();
    q.pop();

    for (pair<int, int> edge : adj[now]) {
      deg[edge.first]--;
      if (deg[edge.first] == 1) {
        q.push(edge.first);
      }
    }
  }
}

void inittreeRoot() {
  for (int i = 1 ; i <= n ; i++) {
    if (deg[i] != 2) continue;

    dfs(i, -1, i);
  }
}

void initCycle() {
  int start = -1;
  for (int i = 1 ; i <= n ; i++) {
    assert(deg[i] <= 2);

    if (deg[i] == 2) {
      start = i;
      break;
    }
  }

  assert(start != -1);

  cycleCnt = 0;
  int cur = start;
  int prv = -1;

  do {
    cycleCnt++;

    pair<int, int> curEdge = {-1, -1};
    for (pair<int, int> edge : adj[cur]) {
      if (deg[edge.first] == 2 && edge.first != prv) {
        curEdge = edge;
      }
    }
    assert(curEdge.first != -1);

    cycleIndex[cur] = cycleCnt;
    cycleVals[cycleCnt] = curEdge.second;

    prv = cur;
    cur = curEdge.first;
  } while (cur != start);

  for (int i = 1 ; i <= cycleCnt ; i++) {
    cycleVals[i + cycleCnt] = cycleVals[i];
  }

  buildSegTree(1, 1, 2 * cycleCnt);
}

int calcSingleTrip(int a, int b) {
  int posA = cycleIndex[a];
  int posB = cycleIndex[b];

  if (posA < posB) {
    return querySegTree(1, 1, 2 * cycleCnt, posA, posB - 1);
  } else {
    return querySegTree(1, 1, 2 * cycleCnt, posA, posB + cycleCnt - 1);
  }
}

int calcInCycle(int a, int b) {
  // why go through cycle if you are already there
  if (a == b) return 1e9;

  int aToB = calcSingleTrip(a, b);
  int bToA = calcSingleTrip(b, a);

  assert(aToB + bToA > 0);
  return aToB + bToA;
}

int solve(int u, int v) {
  if (treeRoot[u] > treeRoot[v]) swap(u, v);

  if (treeRoot[v] == 0) {
    // both in cycle
    
    return calcInCycle(u, v);
  } else if (treeRoot[u] == 0) {
    // one in cycle; go through root and then cycle

    pair<int, int> lca = getLCA(v, 0);
    int inCycle = calcInCycle(u, treeRoot[v]);
    
    return min(lca.second, inCycle);
  } else if (treeRoot[u] == treeRoot[v]) {
    // same treeRoot; must use LCA
    
    pair<int, int> lca = getLCA(u, v);
    return lca.second;
  } else {
    // different treeRoot; go through cycle
    
    pair<int, int> lca = getLCA(u, v);
    int inCycle = calcInCycle(treeRoot[u], treeRoot[v]);

    return min(lca.second, inCycle);
  }

  assert(0);
}

void read() {
  scanf("%d %d", &n, &q);

  for (int i = 1 ; i <= n ; i++) {
    int u, v, w;
    scanf("%d %d %d", &u, &v, &w);

    adj[u].push_back({v, w});
    adj[v].push_back({u, w});
  }
}

void prepare() {
  initDeg();
  inittreeRoot();
  initCycle();
}

void work() {
  for (int i = 0 ; i < q ; i++) {
    int u, v;
    scanf("%d %d", &u, &v);

    printf("%d\n", solve(u, v));
  }
}

int main() {
  read();
  prepare();
  work();
  return 0;
}
