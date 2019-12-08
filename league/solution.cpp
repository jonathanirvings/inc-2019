// INCLUDE LIST
#include <cstdio>
#include <bitset>
#include <iostream>
#include <queue>
#include <stack>
#include <string>
#include <cstring>
#include <algorithm>
#include <map>
#include <set>
#include <list>
#include <vector>
#include <cstdlib>
#include <cctype>
#include <cmath>
#include <iomanip>
using namespace std;

int main() {
  int n, a[1000], b[1000], total = 0;

  cin >> n;
  for (int i = 0; i < n; i++) cin >> a[i];
  for (int i = 0; i < n; i++) cin >> b[i];
  
  for (int i = 0; i < n; i++) {
    total += (a[i] > b[i]) ? 3 : (a[i] < b[i]) ? 0 : 1;
  }
  
  cout << total << endl;
  return 0;
}


