#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <unordered_map>

using namespace std;
// solution in O(n*m) ;)

int n,m,k;

vector<int> v;
// dp[i][l] = max attack when having i attackers and beign at line l
vector<vector<int>> dp;
// bound[l] == -1 if starting at position l is not possible to attack a line
// otherwise it will contain the index r at which the attacking line ends
vector<int> bound;

void testcase(){
  // initialization
  cin >> n >> m >> k;
  v.clear();
  v.resize(n+1,-1);

  dp.clear();
  //dp.resize(n+2,vector<int>(m+1,-1));
  dp.resize(m+1,vector<int>(n+2,-1));
  bound.clear();
  bound.resize(n+1,-1);

  for(int i=1;i<=n;i++){
    cin >> v[i];
  }

  // sliding window for finding valid attack starting points
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
  
  // starting from the end fill the dp
  l=n-1;
  dp[0][n+1] = 0;
  dp[0][n] = 0;
  if(v[n]==k) dp[1][n] = 1;

  while(l>=1){
    dp[0][l] = 0;
    // check if possible to start attacking a line at position l
    int r = bound[l];
    if(r==-1){
      for(int i=1;i<=m;i++){
        dp[i][l] = dp[i][l+1];
      }
    } else {
      for(int i=1;i<=m;i++){
        if(dp[i-1][r+1] == -1) 
          dp[i][l] = dp[i][l+1];
        else
          dp[i][l] = max(dp[i][l+1],(r-l+1)+dp[i-1][r+1]);
      }
    }
    l--;
  }


  int res = dp[m][1];
  if(res==-1) cout << "fail" << endl;
  else        cout << res << endl;
}

  
int main(){
  ios_base::sync_with_stdio(false);
  int t; cin >> t;
  for(int i=0;i<t;i++) testcase();
}
