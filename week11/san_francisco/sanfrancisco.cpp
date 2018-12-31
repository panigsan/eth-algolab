#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

#define trace(x) cerr << #x << " = " << x << endl

vector<vector<pair<int,long>>> G;
vector<vector<long>> dp;

long solve(int node, int moves_left){
  if(moves_left==0) return 0;
  if(dp[node][moves_left] != -1) return dp[node][moves_left];
  long max_points=0;
  if(G[node].size()==0){ // in Weayaya returns to beginning
    max_points = solve(0,moves_left);
  }
  else {
    for(pair<int,long> e : G[node]){
      long points = e.second + solve(e.first,moves_left-1);
      max_points = max(max_points,points);
    }
  }
  dp[node][moves_left] = max_points;
  return max_points;
}
void testcase(){
  int n,m;
  long x,k;
  cin >> n >> m >> x >> k;
  G.clear();
  G.resize(n);
  for(int i=0;i<m;++i){
    int u,v;
    long p;
    cin >> u >> v >> p;
    G[u].push_back(make_pair(v,p));
  }

  int l=1;
  int r=k;
  int c;
  dp.clear();
  dp.resize(n,vector<long>(k+1,-1));
  do {
    c = l+(r-l)/2;
    long t = solve(0,c);
    if(t<x) l = c+1;
    else r=c-1;
  } while (l<=r);

  int t = l+(r-l)/2;
  if(t<=k) cout << t << endl;
  else     cout << "Impossible" << endl;

}

int main(){
  ios_base::sync_with_stdio(false);
  int T;
  cin >> T;
  while(T--) testcase();
}
