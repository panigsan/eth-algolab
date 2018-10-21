#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

#define mp make_pair
#define trace(x) cout << #x << " = " << x << endl
#define pr(x) "(" << x.first << "," << x.second << ")"

int INF = 1<<20; 
//int INF = numeric_limits<int>::max();

void testcase(){
  int n,k; cin >> n >> k;
  vector<int> c(n+1,0);
  vector<int> l(n+1,0);
  for(int i=1;i<=n;++i){
    cin >> c[i];
    cin >> l[i];
  }

  vector<vector<pair<int,int>>> dp(n+1,vector<pair<int,int>>(k+1,{0,0}));

  for(int i=0;i<=n;++i) dp[i][0] = make_pair(0,0);
  for(int i=1;i<=k;++i) dp[0][i] = make_pair(INF,-INF);

  for(int i=1;i<=n;++i){
    for(int j=1;j<=k;++j){
      dp[i][j] = dp[i-1][j];
      for(int z=0;z<j;++z){
        pair<int,int> x = dp[i-1][z];
        int b = ceil((double)(j-z)/(double)l[i]);
        int w = x.first + c[i]* b;
        if(w < dp[i][j].first){
          dp[i][j].first = w;
          dp[i][j].second = x.second + 1;
        } else if(w==dp[i][j].first){
          if(x.second + 1 > dp[i][j].second){
            dp[i][j].second = x.second+1;
          }
        }
      }
    }
  }

  /*
  for(int i=0;i<=n;++i){
    for(int j=0;j<=k;++j){
      cout << pr(dp[i][j]) << "\t";
    }
    cout << endl;
  }
  */

  cout << dp[n][k].first << " " << dp[n][k].second << endl;

}

int main(){
  ios_base::sync_with_stdio(false);
  int T; cin >> T;
  while(T--) testcase();
}
