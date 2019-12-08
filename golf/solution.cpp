// Version 1.0

#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
using namespace std;

typedef long long ll;

int INF = 10000;

ll N, a, b, s, t;

ll P[2005];
ll H[2005];

vector<ll> lines;

ll compute(ll start, int index) {
	// start -> starting point
	// index -> index of the tree
	// exclusive -> the starting point is not x, but start + eps or start - eps based on the direction of the movement

	ll p = P[index];
	ll h = H[index];


	if (p == start) {
		// should not happen
		cout << "ERROR" << endl;
		exit(0);
		return -1;
	}

	bool right; // direction is to the right
	ll pre;
	if (p > start) {
		right = true;
		pre = p - start;
	} else {
		right = false;
		pre = start -p;
	}

	ll h2 = h * h;
	ll post = h2 / pre;

	return post;
}

// return (x, y) inclusive range of areas that are blocked by the trees (RIGHT)
pair<int, int> process_right(ll start, int index) {
	ll post = compute(start, index);

	ll p = P[index];
	ll q = p + post;

	vector<ll>::iterator upper = upper_bound(lines.begin(), lines.end(), q);
	// able: the smallest index of the segment where we can make a semicircle arc
	int able = (upper - lines.begin()) - 1; // able >= 1
	return make_pair(index, able - 1);
}

// MAGIC
pair<int, int> process_left(ll start, int index) {
	ll post = compute(start, index);
	ll p = P[index];
	ll q = p - post;

	vector<ll>::iterator lower_plus1 = lower_bound(lines.begin(), lines.end(), q);
	// the greatest index of the segment where we can make a semicircle arc
	int able = ((lower_plus1 - lines.begin()) - 1);
	return make_pair(able + 1, index - 1);
}

class Checker {
	int segment;
	vector<int> counter;

public:
	Checker(int _segment, int N) {
		segment = _segment;
		for (int x = 0; x < N + 2; ++x) {
			counter.push_back(0);
		}
	}

	void update(int x, int y) {
		counter[x]++;
		counter[y + 1]--;
	}

	vector<int> output() {
		vector<int> result;
		int count = 0;

		for (int x = 0; x < N + 1; ++x) {
			// count when x = N + 2 is always zero, but we do not need it
			count += counter[x];
			if (count == 0) {
				result.push_back(x);
			}
		}

		return result;
	}
};

vector< vector<int> > AdjList;

ll left(int x) {
	return lines[x];
}

ll right(int x) {
	return lines[x + 1];
}

int main () {
	cin >> N >> a >> b >> s >> t;

	// The area in between a, P_1, P_2, ..., P_N, b are denoted as 0, 1, 2, ..., N

	lines.push_back(a);

	for (int i = 1; i <= N; ++i) {
		cin >> P[i];
		lines.push_back(P[i]);
	}

	for (int i = 1; i <= N; ++i) {
		cin >> H[i];
	}

	lines.push_back(b);

	// cek if hole in one is possible

	bool hole_in_one = true;

	for (int i = 1; i <= N; ++i) {
		if (s < P[i] && P[i] < t) {
			// s < t

			ll pre = P[i] - s;
			ll post = t - P[i];
			if (pre * post < H[i] * H[i]) {
				hole_in_one = false;
				break;
			}
		}
	}

	if (hole_in_one) {
		cout << 1 << endl;
		return 0;
	}

	// otherwise, graph

	// s is N + 1, t is N + 2
	AdjList.resize(N + 3, vector<int>());

	for (int x = 0; x <= N; ++x) {
		Checker c = Checker(x, N);
		for (int i = 1; i <= x; ++i) {
			ll start = right(x);
			pair<int, int> interval = process_left(start, i);
			c.update(interval.first, interval.second);
		}
		for (int i = x + 1; i <= N; ++i) {
			ll start = left(x);
			pair<int, int> interval = process_right(start, i);
			c.update(interval.first, interval.second);
		}

		vector<int> result = c.output();
		for (int v = 0; v < result.size(); ++v) {
			if (result[v] != x) {
				AdjList[x].push_back(result[v]);
			}
		}
	}

	// s and t
	Checker cs = Checker(N + 1, N);
	Checker ct = Checker(N + 2, N);
	pair<int, int> interval;
	for (int i = 1; i <= N; ++i) {
		if (P[i] < s) {
			interval = process_left(s, i);
		} else {
			interval = process_right(s, i);
		}
		cs.update(interval.first, interval.second);
	}

	for (int i = 1; i <= N; ++i) {
		if (P[i] < t) {
			interval = process_left(t, i);
		} else {
			interval = process_right(t, i);
		}
		ct.update(interval.first, interval.second);
	}

	vector<int> result;
	result = cs.output();
	for (int v = 0; v < result.size(); ++v) {
		if (result[v] != N + 1) {
			AdjList[N + 1].push_back(result[v]);
			AdjList[result[v]].push_back(N + 1);
		}
	}

	result = ct.output();
	for (int v = 0; v < result.size(); ++v) {
		if (result[v] != N + 2) {
			AdjList[N + 2].push_back(result[v]);
			AdjList[result[v]].push_back(N + 2);
		}
	}	

  vector<int> d(N + 3, INF);
  d[N + 1] = 0;
  queue<int> q;
  q.push(N + 1);
  while(!q.empty()) {
  	int u = q.front();
  	q.pop();
  	for (vector<int>::iterator v = AdjList[u].begin(); v != AdjList[u].end(); ++v) {
  		if (d[*v] == INF) {
  			d[*v] = d[u] + 1;
  			q.push(*v);
  		}
  	}
  }

  if (d[N + 2] == INF) {
  	cout << -1 << endl;
  } else {
  	cout << d[N + 2] << endl;
  }

  return 0;
}