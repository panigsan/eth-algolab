#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <algorithm>

using namespace std;

int min_dist(int);
int max_dist(int);

int n;
int m;
int r;
int b;
vector<vector<int>> g;
vector<int> dp_min;
vector<int> dp_max;
void testcase(){
  cin >> n >> m >> r >> b;

  g.clear();
  dp_min.clear();
  dp_max.clear();
  for(int i=0;i<=n;i++){
    g.push_back({});
    dp_min.push_back(-1);
    dp_max.push_back(-1);
  }
  for(int i=1;i<=m;i++){
    int u; cin >> u;
    int v; cin >> v;
    g[u].push_back(v);
  }

  int min_r = min_dist(r);
  int min_b = min_dist(b);
  // win if either less OR equal but red arrived first
  if(min_r<min_b || (min_r==min_b && (min_r%2==1))) cout << 0 << endl;
  else                           cout << 1 << endl;
}

int min_dist(int pos){
  if(pos==n) return 0;
  if(dp_min[pos]!=-1) return dp_min[pos];

  int best = numeric_limits<int>::max();
  for(int next : g[pos]){
    best = min(best,1+max_dist(next));
  }
  dp_min[pos] = best;
  return best;
}

int max_dist(int pos){
  if(pos==n) return 0;
  if(dp_max[pos]!=-1) return dp_max[pos];

  int best = -1;
  for(int next : g[pos]){
    best = max(best,1+min_dist(next));
  }
  dp_max[pos] = best;
  return best;
}

int main(){
  ios_base::sync_with_stdio(false);
  int t; cin >> t;
  for(int i=0;i<t;i++) testcase();
  return 0;
}
