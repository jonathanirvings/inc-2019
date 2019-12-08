#pragma GCC optimize "-O2"
#include <bits/stdc++.h>
using namespace std;

int N, A;
int odd = 0;
int even = 0;

int main(int argc, char *argv[]) {
	scanf( "%d", &N );
	for ( int i = 0; i < N; i++ ) {
		scanf( "%d", &A );
		if ( A % 2 != 0 ) odd++; else even++;
	}
	// pair each odd number with an even number
	// pair the remaining odd numbers to each other
	int ans = min(odd,even) + max(0,(odd-even)/2);
	printf( "%d\n", ans );
	return 0;
}
