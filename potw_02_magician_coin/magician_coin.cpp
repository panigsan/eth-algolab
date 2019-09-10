#include <iostream>
#include <vector>
#include <iomanip>

using namespace std;

double solve(int,int);
int n,k,m;
vector<double> p;
vector<vector<double>> dp;
void testcase(){
  cin >> n >> k >> m;
  p.clear();
  dp.clear();

  for(int i=0;i<=n;i++){
    dp.push_back(vector<double>(m+1,-1.0));
  }

  p.push_back(-1.0);
  for(int i=1;i<=n;i++){
    double pi; cin >> pi;
    p.push_back(pi);
  }

  double sol = solve(1,k);

  cout << std::fixed << std::setprecision(5);
  cout << sol << endl;
}

double solve(int d, int mon_left){
  if(d==n+1 && mon_left>=m) return 1.0;
  if(d==n+1) return 0.0;
  if(mon_left>=m) return 1.0;
  if(dp[d][mon_left] != -1.0) return dp[d][mon_left];

  double s = 0.0;
  for(int b=0;b<=mon_left;b++){
    s = max(s,p[d]*solve(d+1,mon_left+b) + (1.0-p[d])*solve(d+1,mon_left-b));
  }

  dp[d][mon_left] = s;
  return s;
}

int main(){
  int t; cin >> t;
  for(int i=0;i<t;i++) testcase();
}
