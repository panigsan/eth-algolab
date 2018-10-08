#include <iostream>
#include <cassert>
#include <vector>
#include <set>
#include <utility>
#include <algorithm>

using namespace std;

int solve(int,int);
int solve();

int n,m,k;

vector<int> v;
vector<vector<int>> dp;
vector<int> sums;
void testcase(){
  cin >> n >> m >> k;
  v.clear();
  dp.clear();
  sums.clear();
  v.push_back(-1);
  for(int i=1;i<=n;i++){
    int vi; cin >> vi;
    v.push_back(vi);
  }

  
  for(int i=0;i<=n+1;i++){
    dp.push_back(vector<int>(m+1,-1));
  }

  sums.push_back(0);
  for(int i=1;i<=n;i++){
    sums.push_back(sums[i-1] + v[i]);
  }

  int l=n-1;
  dp[n+1][0] = 0;
  dp[n][0] = 0;
  if(v[n]==k) dp[n][1] = 1;

  while(l>=1){
    dp[l][0] = 0;
    auto it = find(sums.begin(),sums.end(),sums[l-1]+k);
    if(it==sums.end()){
      //cout << "l: " << l << " not found " << endl;
      for(int i=1;i<=m;i++){
        dp[l][i] = dp[l+1][i];
      }
    } else {
      int r = it-sums.begin();
      //cout << "l: " << l << " found at " << r << endl;
      for(int i=1;i<=m;i++){
        if(dp[r+1][i-1] == -1) 
          dp[l][i] = dp[l+1][i];
        else
          dp[l][i] = max(dp[l+1][i],(r-l+1)+dp[r+1][i-1]);
      }
    }
    /*
    int r=l;
    int s=v[r];
    while(s<k && r<n){
      r++;
      s += v[r];
    }
    dp[l][0] = 0;
    for(int i=1;i<=m;i++){
      if(s==k && dp[r+1][i-1]!=-1){
        dp[l][i] = max(dp[l+1][i],(r-l+1)+dp[r+1][i-1]);
      } else {
        dp[l][i] = dp[l+1][i];
      }
    }
    */
    l--;
  }


  int res = dp[1][m];
  if(res==-1) cout << "fail" << endl;
  else        cout << res << endl;
}

  
int main(){
  int t; cin >> t;
  for(int i=0;i<t;i++) testcase();
}
