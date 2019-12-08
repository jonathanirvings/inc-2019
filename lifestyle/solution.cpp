#pragma GCC optimize "-O2"
#include <bits/stdc++.h>
using namespace std;
typedef pair<int,int> pii;
typedef vector<int> vi;

const int maxn = 100005;

int N, M, Q;
vi  con[maxn];

int idx = 0;
int vis[maxn] = {0};
int low[maxn] = {0};
set <pii> bridge;

void add_bridge(int a, int b) {
	bridge.insert(pii(min(a,b),max(a,b)));
}
bool is_bridge(int a, int b) {
	return bridge.find(pii(min(a,b),max(a,b))) != bridge.end();
}

void find_bridge(int x, int p = -1) {
	low[x] = vis[x] = ++idx;
	for ( auto &to : con[x] ) {
		if ( vis[to] == 0 ) {
			find_bridge(to,x);
			low[x] = min(low[x],low[to]);
			if ( low[to] > vis[x] ) add_bridge(x,to);
		}
		else if ( to != p )
			low[x] = min(low[x],vis[to]);
	}
}


int p[maxn];
int r[maxn];
void reset(int n=maxn-1) {
	for ( int i = 0; i <= n; i++ ) p[i] = i, r[i] = 0;
}
int find(int x) {
	if ( p[x] != x ) p[x] = find(p[x]);
	return p[x];
}
void link(int x, int y) {
	x = find(x), y = find(y);
	if ( x == y ) return;
	if ( r[x] == r[y] ) r[x]++;
	if ( r[x]  < r[y] ) p[x] = y; else p[y] = x;
}


int main(int argc, char *argv[]) {
	scanf( "%d %d %d", &N, &M, &Q );
	while ( M-- ) {
		int a, b;
		scanf( "%d %d", &a, &b );
		con[a].push_back(b);
		con[b].push_back(a);
	}

	find_bridge(1);

	reset(N);
	for ( int i = 1; i <= N; i++ )
		for ( auto &next : con[i] )
			if ( !is_bridge(i,next) ) link(i,next);

	while ( Q-- ) {
		int a, b;
		scanf( "%d %d", &a, &b );
		printf( "%s\n", find(a) == find(b) ? "YES" : "NO" );
	}

	return 0;
}
