#include <iostream>
#include <cassert>
#include <vector>

using namespace std;

int solve(int,int);

int n,m,k;

vector<int> v;
vector<int> dp;
void testcase(){
  cin >> n >> m >> k;
  v.clear();
  dp.clear();
  v.push_back(-1);
  for(int i=1;i<=n;i++){
    int vi; cin >> vi;
    v.push_back(vi);
  }

  
  for(int i=0;i<=n;i++){
    dp.push_back(-1);
    //dp.push_back(vector<int>(m+1,-1));
  }
  /*
  for(int i=0;i<=n;i++){
    for(int j=0;j<m;j++){
      assert(dp[i][j]==-1);
    }
  }*/

  int res = solve(1,m);
  if(res==-1) cout << "fail" << endl;
  else        cout << res << endl;
  //cout << "--" << endl;
}

int solve(int l, int a){
  if(a==0) return 0;
  if(l==n+1) return -1;
  if(dp[l] != -1) return min(a,dp[l]); //cout << "l: " << l << " a " << a << endl;
  //if(l==n && v[n]==k) return 1;

  int best = -1;
  int border=l;
  int i=l;
  while(border<=n){
    i = border;
    int s = v[i];
    while(s<k && i<n){
      i++;
      s += v[i];
    }
    if(s==k){
      //cout << "- l: " << l << " a: " << a << " i: " << i << endl;
      int res = solve(i+1,a-1);
      if(res!=-1){
        best = max(best,(i-border+1) +res);
      }
    }
    border++;
  }
  dp[l] = best;

  //cout << "l: " << l << " a: " << a << " best: " << best << endl;
  return best;
}

int main(){
  int t; cin >> t;
  for(int i=0;i<t;i++) testcase();
}
