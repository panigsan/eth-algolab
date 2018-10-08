#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <map>

using namespace std;
// solution in O(n*m) ;)

int n,m,k;

vector<int> v;
// dp[l][i] = max attack when at line l and still have i attackers
vector<vector<int>> dp;
// just a partial sum
vector<int> sums;
// stores (sum,index_of_the_sum) for very fast access ;) 
map<int,int> m_sums;

void testcase(){
  cin >> n >> m >> k;
  v.clear();
  dp.clear();
  sums.clear();
  m_sums.clear();

  // initialization
  v.push_back(-1);
  sums.push_back(0);
  dp.push_back(vector<int>(m+1,-1));
  for(int i=1;i<=n;i++){
    int vi; cin >> vi;
    v.push_back(vi);

    sums.push_back(sums[i-1] + v[i]);
    m_sums.insert(make_pair(sums[i],i));
    dp.push_back(vector<int>(m+1,-1));
  }
  
  int l=n-1;
  dp[n+1][0] = 0;
  dp[n][0] = 0;
  if(v[n]==k) dp[n][1] = 1;

  // starting from the end fill the dp
  while(l>=1){
    dp[l][0] = 0;
    // check if possible to start attacking a line at position l
    auto it=m_sums.find(sums[l-1]+k);
    if(it==m_sums.end()){
      for(int i=1;i<=m;i++){
        dp[l][i] = dp[l+1][i];
      }
    } else {
      int r = it->second;
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
  int t; cin >> t;
  for(int i=0;i<t;i++) testcase();
}
