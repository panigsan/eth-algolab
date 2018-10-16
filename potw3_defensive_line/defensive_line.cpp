#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <unordered_map>

using namespace std;
// solution in O(n*m) ;)

int n,m,k;

vector<int> v;
// dp[l][i] = max attack when at line l and still have i attackers
vector<vector<int>> dp;
// stores (sum,index_of_the_sum) for very fast access ;) 
vector<int> bound;

void testcase(){
  cin >> n >> m >> k;
  v.clear();
  v.resize(n+1,-1);

  dp.clear();
  dp.resize(n+2,vector<int>(m+1,-1));
  bound.clear();
  bound.resize(n+1,-1);

  // initialization
  for(int i=1;i<=n;i++){
    cin >> v[i];
  }
  
  // bound[l] == -1 if starting at position l is not possible to attack a line
  // otherwise it will contain the index r at which the attacking line ends
  int l=1,r=1;
  int s=v[1];
  while(r<=n){
    if(s==k){
      bound[l] = r;
      s-=v[l];
      l++;
    }else if(s<k){
      r++;
      s+=v[r];
    } else if(s>k){
      s-=v[l];
      l++;
    }
  }
  
  l=n-1;
  dp[n+1][0] = 0;
  dp[n][0] = 0;
  if(v[n]==k) dp[n][1] = 1;

  // starting from the end fill the dp
  while(l>=1){
    dp[l][0] = 0;
    // check if possible to start attacking a line at position l
    int r = bound[l];
    if(r==-1){
      for(int i=1;i<=m;i++){
        dp[l][i] = dp[l+1][i];
      }
    } else {
      for(int i=1;i<=m;i++){
        if(dp[r+1][i-1] == -1) 
          dp[l][i] = dp[l+1][i];
        else
          dp[l][i] = max(dp[l+1][i],(r-l+1)+dp[r+1][i-1]);
      }
    }
    l--;
  }


  int res = dp[1][m];
  if(res==-1) cout << "fail" << endl;
  else        cout << res << endl;
}

  
int main(){
  ios_base::sync_with_stdio(false);
  int t; cin >> t;
  for(int i=0;i<t;i++) testcase();
}
