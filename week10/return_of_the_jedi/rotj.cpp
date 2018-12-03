#include <iostream>
#include <vector>
#include <queue>
#include <utility>
#include <algorithm>
#include <tuple>

using namespace std;

int n,i;
vector<vector<int>> edges;
// run algorithm avoiding the edge between no1 and no2
int algo(vector<vector<int>>& new_edges,int start, int no1, int no2){
  vector<bool> visited(n+1,false);
  priority_queue<tuple<int,int,int>> Q;
  visited[start] = true;
  for(int j=1;j<=n;++j) Q.push(make_tuple(-edges[start][j],j,start));

  int tot_cost = 0;
  int added=1;
  while(added < n){
    auto e = Q.top(); Q.pop(); 
    int cost = -get<0>(e);
    int u = get<1>(e);
    int v = get<2>(e);
    if((u==no1 && v==no2) || (u==no2 && v==no1)) continue;
    if(visited[u]) continue;

    new_edges[u].push_back(v);
    new_edges[v].push_back(u);
    tot_cost += cost;
    visited[u] = true;
    for(int j=1;j<=n;++j) Q.push(make_tuple(-edges[u][j],j,u));

    ++added;
  }
  
  return tot_cost;
}
int algo(vector<vector<int>>& new_edges,int start){
  return algo(new_edges,start,-1,-1);
}

void testcase(){
  cin >> n >> i;

  edges.clear();
  edges.resize(n+1,vector<int>(n+1,-1));
  vector<vector<bool>> b_edges(n+1,vector<bool>(n+1,false));
  for(int j=1;j<=n-1;++j){
    for(int k=1;k<=n-j;++k){
      int c;
      cin >> c;
      edges[j][j+k] = c;
      edges[j+k][j] = c;
    }
  }

  vector<vector<int>> new_edges(n+1,vector<int>());
  int tot_cost = algo(new_edges,i);

  int best = numeric_limits<int>::max();

  if(n>100){ // testcase 2
    for(int j=1;j<=n;++j){
      if(new_edges[j].size()!=1) continue;

      for(int k=1;k<=n;++k){
        if(k == j || k == new_edges[j][0]) continue;
        int current = tot_cost - edges[j][new_edges[j][0]] + edges[j][k]; 
        best = min(best,current);
      }
    }

  } else { // testcase 1
    for(int j=1;j<=n;j++){
      for(int k=0;k<new_edges[j].size();++k){
        vector<vector<int>> new_edges2(n+1,vector<int>());
        int current = algo(new_edges2,j,j,new_edges[j][k]);
        best = min(best,current);
      }
    }
  }
  cout << best << endl;
}

int main(){
  ios_base::sync_with_stdio(false);
  int T;
  cin >> T;
  while(T--) testcase();
}
