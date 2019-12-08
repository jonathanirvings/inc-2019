#pragma GCC optimize "-O2"
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef pair<int,int> pii;

class BIT {
	public:
	int n;
	ll data[1000005] = {0};
	BIT(int nn=1000000) { n = nn+1; }
	void add(int x, ll val) {
		x++; while ( x <= n ) { data[x] += val; x += x & -x; }
	}
	ll sum(int x) {
		ll ret = 0;
		x++; while ( x > 0 ) { ret += data[x]; x -= x & -x; }
		return ret;
	}
	ll range_sum(int L, int R) { return sum(R) - sum(L-1); }
};

BIT bit;
set <pii> segments;
bool no_stone[1000005] = {false};

/** cost per segment (n^2) */
ll f(const pii s) {
	ll ret = s.second - s.first + 1;
	return ret * ret;
}

void add_segment(const pii s) {
	segments.insert(s);
	bit.add(s.first,f(s));
}

void remove_segment(const pii s) {
	segments.erase(s);
	bit.add(s.first,-f(s));
}


/** stone removal */
void putra_rem(int x) {
	pii stone = pii(x,x);
	if ( no_stone[x-1] && no_stone[x+1] ) {
		/** merge with two neigboring segments */
		set <pii>::iterator it = segments.lower_bound(stone);
		set <pii>::iterator pr = std::prev(it);
		pii new_segment = pii(pr->first,it->second);
		remove_segment(*it);
		remove_segment(*pr);
		add_segment(new_segment);
	}
	else if ( no_stone[x-1] ) {
		/** merge with the previous segment */
		set <pii>::iterator it = segments.lower_bound(stone);
		set <pii>::iterator pr = std::prev(it);
		pii new_segment = pii(pr->first,x);
		remove_segment(*pr);
		add_segment(new_segment);
	}
	else if ( no_stone[x+1] ) {
		/** merge with the next segment */
		set <pii>::iterator it = segments.lower_bound(stone);
		pii new_segment = pii(x,it->second);
		remove_segment(*it);
		add_segment(new_segment);
	}
	else {
		/** standalone missing stone */
		pii new_segment = stone;
		add_segment(new_segment);
	}
	no_stone[x] = true;
}

/** stone addition */
void putra_add(int x) {
	pii stone = pii(x,1000000001);
	set <pii>::iterator it = std::prev(segments.upper_bound(stone));
	pii LS = pii(it->first,x-1);
	pii RS = pii(x+1,it->second);
	remove_segment(*it);
	if ( LS.first <= LS.second ) add_segment(LS);
	if ( RS.first <= RS.second ) add_segment(RS);
	no_stone[x] = false;
}

/** a-b query */
ll putra_query(int a, int b) {
	return bit.range_sum(a,b);
}


/** for compression */
int n_id = 0;
set <int> pos;
map <int,int> id;
void add_position(int x) {
	pos.insert(x);
	pos.insert(x-1);
	pos.insert(x+1);
}


/** for input data */
int N, M, Q;
int p[100005];
struct tquery { int type, a, b; };
tquery query[100005];


int main(int argc, char *argv[]) {

	/** read all input */
	char cmd[10];
	scanf( "%d %d %d", &N, &M, &Q );
	for ( int i = 0; i < M; i++ )
		scanf( "%d", &p[i] );
	for ( int i = 0; i < Q; i++ ) {
		scanf( "%s", cmd );
		if ( cmd[0] == 'a' ) {
			query[i].type = 0;
			scanf( "%d", &query[i].a );
		}
		if ( cmd[0] == 'r' ) {
			query[i].type = 1;
			scanf( "%d", &query[i].a );
		}
		if ( cmd[0] == 'g' ) {
			query[i].type = 2;
			scanf( "%d %d", &query[i].a, &query[i].b );
			if ( query[i].a > query[i].b )
				swap(query[i].a,query[i].b);
		}
	}
	
	/** compress all positions which appear in input */
	/**   for each x, mark [x-1, x, x+1]  */
	for ( int i = 0; i < M; i++ )
		add_position(p[i]);
	for ( int i = 0; i < Q; i++ ) {
		add_position(query[i].a);
		if ( query[i].type == 2 )
			add_position(query[i].b);
	}

	/** re-map all positions */
	pos.erase(0);
	for ( auto &x : pos )
		id[x] = ++n_id;
	for ( int i = 0; i < M; i++ )
		p[i] = id[p[i]];
	for ( int i = 0; i < Q; i++ ) {
		query[i].a = id[query[i].a];
		if ( query[i].type == 2 )
			query[i].b = id[query[i].b];
	}
	
	/** solve */
	for ( int i = 0; i < M; i++ )
		putra_rem(p[i]);
	for ( int i = 0; i < Q; i++ ) {
		if ( query[i].type == 0 ) 
			putra_add(query[i].a);
		else if ( query[i].type == 1 )
			putra_rem(query[i].a);
		else if ( query[i].type == 2 )
			printf( "%lld\n", putra_query(query[i].a+1,query[i].b-1));
	}

	return 0;
}
