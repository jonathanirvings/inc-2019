#include <stdio.h>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <string>
#include <algorithm>
#include <vector>
#include <deque>
#include <queue>
#include <stack>
#include <map>
#include <set>
#include <limits.h>
#include <time.h>
#include <bitset>
#include <list>
#include <cassert>
 
#define EPS 1e-11
#define PI acos(-1)
#define LL long long
#define INF 1000000007
#define MP(a,b) make_pair(a,b)
#define PB(a) push_back(a)
#define SZ(a) ((int)a.size())
#define OPENR(a) freopen(a,"r",stdin)
#define OPENW(a) freopen(a,"w",stdout)
#define pii pair<int,int>
 
int x4[4] = { 0, 0,-1, 1};
int y4[4] = {-1, 1, 0, 0};
int x8[] = {-1,-1,-1, 0, 0, 1, 1, 1,0};
int y8[] = {-1, 0, 1,-1, 1,-1, 0, 1,0};
int xhorse[8] = {1,2,1,2,-1,-2,-1,-2};
int yhorse[8] = {2,1,-2,-1,2,1,-2,-1};
 
using namespace std;

int n, h;
int p[105], v[105];
LL dp[105][10005];

LL maxPower(int pos, int lossAllowed) {
	// when the position is 0, reset dp
	if (pos == 0) memset(dp, -1, sizeof(dp));
	
	// end of item list, or cannot take anymore
	if (pos == n || lossAllowed == 0) return 0;
	
	if (dp[pos][lossAllowed] != -1) return dp[pos][lossAllowed];
	
	LL power = p[pos];
	int value = v[pos];
	LL &res = dp[pos][lossAllowed];
	res = maxPower(pos+1, lossAllowed); // not take current item
	
	// take current item multiple times
	LL prevPower = 0;
	
	// as long as the item is still ok (not destroyed), and we are still looking for higher res
	while(value > 0 && res < h) {
		int cost = v[pos] - value / 2;
		if (lossAllowed - cost < 0) break;

		res = max(res, power + prevPower + maxPower(pos + 1, lossAllowed - cost));
		prevPower += power;
		power *= 2;
		value /= 2;
	}

	return res;
}

int main() {
	scanf("%d %d", &n, &h);
	
	int vTotal = 0;
	for (int i=0;i<n;i++) {
		scanf("%d %d", &p[i], &v[i]);
		vTotal += v[i];
	}
	
	// default to -1
	int ans = -1;
	
	// binary search the answer, minimum answer is 1, max is sum(v)
	int l = 1, r = vTotal;
	while (true) {
		if (r - l <= 1) {
			// check l
			if (maxPower(0, l) >= h) {
				ans = l;
			} else if (maxPower(0, r) >= h){ // check r
				ans = r;
			}
			break;
		}
		
		int mid = (l + r) / 2;
		if (maxPower(0, mid) >= h) {
			r = mid;
		} else {
			l = mid;
		}
	}
	
	printf("%d\n", ans);
	
	return 0;
}

