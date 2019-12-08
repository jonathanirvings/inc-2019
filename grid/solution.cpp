#include <bits/stdc++.h>

using namespace std;

/** Max Flow Min Cost **/
/* complexity: O(min(E^2 V log V, E log V F)) */
struct MCMF {
  const static int maxnodes = 5010;
  typedef long long ll;

  int nodes = maxnodes;
  int prio[maxnodes], curflow[maxnodes], prevedge[maxnodes], prevnode[maxnodes], q[maxnodes], pot[maxnodes];
  bool inqueue[maxnodes];

             
  struct Edge {
    int to, f, cap, cost, rev;
  };

  vector<Edge> graph[maxnodes];

  void addEdge(int s,int t,int cap,int cost){
    Edge a ={t,0, cap, cost, (int)graph[t].size()};
    Edge b ={s,0,0,-cost, (int)graph[s].size()};
    graph[s].push_back(a);
    graph[t].push_back(b);
  }

  void bellmanFord(int s,int dist[]){
    fill(dist, dist + nodes,1000000000);
    dist[s]=0;
    int qt =0;
    q[qt++]= s;
    for(int qh =0;(qh - qt)% nodes !=0; qh++){
      int u = q[qh % nodes];
      inqueue[u]=false;
      for(int i =0; i <(int) graph[u].size(); i++){
        Edge &e = graph[u][i];
        if(e.cap <= e.f)continue;
        int v = e.to;
        int ndist = dist[u]+ e.cost;
        if(dist[v]> ndist){
          dist[v]= ndist;
          if(!inqueue[v]){
            inqueue[v]=true;
            q[qt++% nodes]= v;
          }
        }
      }
    }
  }


  pair<int, int> minCostFlow(int s,int t,int maxf){
    // bellmanFord can be safely commented if edges costs are non-negative
    bellmanFord(s, pot);
    int flow =0;
    int flowCost =0;
    while(flow < maxf){
      priority_queue<ll, vector<ll>, greater<ll>> q;
      q.push(s);
      fill(prio, prio + nodes,1000000000);
      prio[s]=0;
      curflow[s]=1000000000;
      while(!q.empty()){
        ll cur = q.top();
        int d = cur >>32;
        int u = cur;
        q.pop();
        if(d != prio[u])continue;
        for(int i =0; i <(int) graph[u].size(); i++){
          Edge &e = graph[u][i];
          int v = e.to;
          if(e.cap <= e.f)continue;
          int nprio = prio[u]+ e.cost + pot[u]- pot[v];
          if(prio[v]> nprio){
            prio[v]= nprio;
            q.push(((ll) nprio <<32)+ v);
            prevnode[v]= u;
            prevedge[v]= i;
            curflow[v]= min(curflow[u], e.cap - e.f);
          }
        }
      }
      if(prio[t]==1000000000)break;
      for(int i =0; i < nodes; i++) pot[i]+= prio[i];
      int df = min(curflow[t], maxf - flow);
      flow += df;
      for(int v = t; v != s; v = prevnode[v]){
        Edge &e = graph[prevnode[v]][prevedge[v]];
        e.f += df;
        graph[v][e.rev].f -= df;
        flowCost += df * e.cost;
      }
    }
    return make_pair(flow, flowCost);
  }
};
/* usage example:
 * addEdge (source, target, capacity, cost)
 * minCostFlow(source, target, INF) -><flow, flowCost>
 */

int main() {
  auto startt = clock();
  int r, c;
  scanf("%d %d", &r, &c);
  vector<int> node_ver(r * c, -1), node_hor(r * c, -1);
  vector<vector<int>> cost(r * c, vector<int>(4, 0));
  int nodes = 2;
  int st = 0, en = 1;
  MCMF mcmf;
  int tot_out = 0, tot_in = 0;
  for (int i = 0; i < r; ++i) {
    for (int j = 0; j < c; ++j) {
      char s[5];
      scanf("%s", s);
      int now = i * c + j;
      int cnt = 0;
      for (int k = 0; k < 4; ++k) {
        if (s[k] == '1') {
          ++cnt;
        }
      }
      if (cnt == 2) {
        assert(s[0] != s[2] && s[1] != s[3]);
        node_ver[now] = nodes++;
        node_hor[now] = nodes++;
        for (int k = 0; k < 4; ++k)
          cost[now][k] = 3 * (s[k] == '0');
        if ((i + j) & 1) {
          mcmf.addEdge(st, node_hor[now], 1, 0);
          mcmf.addEdge(st, node_ver[now], 1, 0);
          tot_in += 2;
        }
        else {
          mcmf.addEdge(node_hor[now], en, 1, 0);
          mcmf.addEdge(node_ver[now], en, 1, 0);
          tot_out += 2;
        }
      }
      else {
        node_hor[now] = node_ver[now] = nodes++;
        if (cnt == 1) {
          int pos = -1;
          for (int k = 0; k < 4; ++k) {
            if (s[k] == '1')
              pos = k;
            cost[now][k] = 3;
          }
          cost[now][pos] = 0;
          cost[now][pos^2] = 6;
        }
        else if (cnt == 3) {
          int pos = -1;
          for (int k = 0; k < 4; ++k) {
            if (s[k] == '0')
              pos = k;
            cost[now][k] = 1;
          }
          cost[now][pos] = 4;
          cost[now][pos^2] = -2;
        }
        if ((i + j) & 1) {
          mcmf.addEdge(st, node_hor[now], cnt, 0);
          tot_in += cnt;
        }
        else {
          mcmf.addEdge(node_hor[now], en, cnt, 0);
          tot_out += cnt;
        }
      }
    }
  }
  if (tot_in != tot_out) {
    puts("-1");
    return 0;
  }
  for (int i = 0; i < r; ++i) {
    for (int j = 0; j < c; ++j) {
      if (i + 1 < r) {
        int u = i * c + j, v = u + c;
        int sum_cost = cost[u][2] + cost[v][0];
        if (((i + j) & 1) == 0)
          swap(u, v);
        mcmf.addEdge(node_ver[u], node_ver[v], 1, sum_cost);
      }
      if (j + 1 < c) {
        int u = i * c + j, v = u + 1;
        int sum_cost = cost[u][1] + cost[v][3];
        if (((i + j) & 1) == 0)
          swap(u, v);
        mcmf.addEdge(node_hor[u], node_hor[v], 1, sum_cost);
      }
    }
  }
  auto result = mcmf.minCostFlow(st, en, tot_in);
  if (result.first != tot_in) {
    puts("-1");
    return 0;
  }
  assert((result.second % 3) == 0 && result.second >= 0);
  int ans = result.second / 3;
  printf("%d\n", ans);
  return 0;
}
