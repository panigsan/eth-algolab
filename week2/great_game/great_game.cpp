#include <iostream>
#include <vector>

using namespace std;

bool solve(int,int,int,int);

int n;
int m;
int r;
int b;
vector<vector<int>> g;
vector<vector<int>> dp[2][2];
void testcase(){
  cin >> n >> m >> r >> b;

  for(int player=0;player<=1;player++){
    for(int parity=0;parity<=1;parity++){
      dp[player][parity].clear();
      dp[player][parity] = vector<vector<int>>(n+1,vector<int>(n+1,-1));
    }
  }

  g.clear();
  for(int i=0;i<=n;i++){
    g.push_back({});
  }
  for(int i=1;i<=m;i++){
    int u; cin >> u;
    int v; cin >> v;
    g[u].push_back(v);
  }

  bool t = solve(r,b,0,0);
  if(t) cout << 0 << endl;
  else  cout << 1 << endl;
}

// player = 0 == sherlok
bool solve(int pos_r, int pos_b, int player, int parity){
  if(player==0 && pos_r==n) return true;
  if(player==0 && pos_b==n) return false;
  if(player==1 && pos_b==n) return true;
  if(player==1 && pos_r==n) return false;
  
  if(dp[player][parity][pos_r][pos_b] != -1){
    return dp[player][parity][pos_r][pos_b];
  }

  int ret_val = false;

  if(player==0){
    if(parity==0){
      for(int new_r : g[pos_r]){
        bool t = solve(new_r,pos_b,1,0);
        if(!t){
          ret_val = true;
          break;
        }
      }
    } else {
      for(int new_b : g[pos_b]){
        bool t = solve(pos_r,new_b,1,1);
        if(!t){
          ret_val = true;
          break;
        }
      }
    }
  } else {
    if(parity==0){
      for(int new_b : g[pos_b]){
        bool t = solve(pos_r,new_b,0,1);
        if(!t){
          ret_val = true;
          break;
        }
      }
    } else {
      for(int new_r : g[pos_r]){
        bool t = solve(new_r,pos_b,0,0);
        if(!t){
          ret_val = true;
          break;
        }
      }
    }
  }
  dp[player][parity][pos_r][pos_b] = ret_val;
  return ret_val;
}

int main(){
  ios_base::sync_with_stdio(false);
  int t; cin >> t;
  for(int i=0;i<t;i++) testcase();
  return 0;
}
