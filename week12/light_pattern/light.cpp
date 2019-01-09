#include <iostream>
#include <vector>
#include <bitset>
#include <algorithm>

using namespace std;

#define B 100000
#define trace(x) cerr << #x << " = " << x << endl
#define btrace(x) cerr << #x << " = " << bitset<2>(x) << endl
#define one bitset<B>(1)

bitset<B> l;
vector<int> dp;
long solve(int i, bool inv){
  if(i==-1) return 0;
  long idx = (i<<1) | inv;
  if(dp[idx]!=-1) return dp[idx];

  long best;
  // if no need to invert, then goes to next one
  if(inv==l.test(i)) best = solve(i-1,inv);
  // otherwise try the two options
  else best = min(1+solve(i-1,inv),1+solve(i-1,!inv));

  dp[idx] = best;
  return best;
}

void testcase(){
  int n,k,x;
  cin >> n >> k >> x;
  l = 0;
  for(int i=1;i<=n;++i){
    bool b;
    cin >> b;
    l.set(i,b);
  }

  dp.clear();
  dp.resize(n*2,-1);

  long cnt = solve(n-1,false);
  cout << cnt << endl;

}

int main(){
  ios_base::sync_with_stdio(false);
  int T;
  cin >> T;
  while(T--) testcase();
}
