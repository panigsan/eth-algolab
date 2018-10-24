#include <iostream>
#include <vector>

using namespace std;

// Debug only
#define step(x)  cout << "Step " << x << endl
#define trace(x) //cout << #x << " = " << x << endl
#define pr(x)    "(" << x.first << "," << x.second << ")"

// A very very large number
int INF = 1<<22; 

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

      // Try using the current line or the previous line z=[0,1]
      for(int z=0; z<=1; ++z){
        pair<int,int> x = dp[i-z][max(0,j-l[i])];
        if(x.first + c[i] < dp[i][j].first){
          dp[i][j].first = x.first+c[i];
          dp[i][j].second = x.second + z;
        } else if (x.first + c[i] == dp[i][j].first){
          dp[i][j].second = max(dp[i][j].second, x.second+z);
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
