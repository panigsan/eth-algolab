#include <iostream>
#include <vector>
#include <bitset>
#include <algorithm>

using namespace std;

#define B 100000

int n,k,x;
bitset<B> l;
bitset<B> bx;
vector<int> dp;
long solve(int i, bool inv){
  if(i==-1) return 0;
  long idx = (i<<1) | inv;
  if(dp[idx]!=-1) return dp[idx];

  // number of changes to the pattern
  long changes_to_norm = 0;
  for(int j=0;j<k;++j){
    changes_to_norm += bx.test(j) == l.test(i*k+k-j-1);
  }
  if(!inv) changes_to_norm = k-changes_to_norm;

  // either change the single bits of the current row OR
  // change them in the opposite way and flip the whole rows
  long best = min(changes_to_norm+solve(i-1,inv),(k-changes_to_norm) + 1 +solve(i-1,!inv));

  dp[idx] = best;
  return best;
}

void testcase() {
  cin >> n >> k >> x;
  bx = bitset<B>(x);
  l = 0;
  for(int i=0;i<n;++i){
    bool b;
    cin >> b;
    l.set(i,b);
  }

  dp.clear();
  dp.resize(n*2,-1);

  long cnt = solve(n/k-1,false);
  cout << cnt << endl;
}

int main(){
  ios_base::sync_with_stdio(false);
  int T;
  cin >> T;
  while(T--) testcase();
}
