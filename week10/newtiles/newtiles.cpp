#include <iostream>
#include <vector>
#include <bitset>

#define trace(x)  //cerr << #x << " = " << x << endl
#define btrace(x) //cerr << #x << " = " << bitset<6>(x) << endl

using namespace std;

// Return number of even ones or -1 
int count(int m){
  int cnt = 0;
  while(m!=0){
    m >>= __builtin_ctz(m);
    if((m&3) != 3) return -1;
    m >>= 2;
    ++cnt;
  }
  return cnt;
}

void testcase(){
  int h,w;
  cin >> h >> w;
  vector<int> rows(h,0);
  for(int i=0;i<h;++i){
    for(int j=0;j<w;++j){
      int x;
      cin >> x;
      rows[i] |= (x<<j);
    } 
    rows[i] >>= 1;
  }
  w = max(0,w-2);

  // used for early stopping
  int current_best = 0;
  vector<vector<int>> dp(h,vector<int>(1<<w,0));
  for(int i=1;i<h;++i){
    for(int m_in=0;m_in < (1<<w); ++m_in){
      int m = m_in & rows[i] & rows[i-1];
      // skip if this possibility was already computed
      if(dp[i][m]!=0) continue;

      int cnt = count(m);
      // skip if ones are not in pairs
      if(cnt==-1) continue;

      // inverse map of m
      int im = ~m & ((1<<w)-1);

      int best = 0;
      int m_up = 0;
      do {
        best = max(best,dp[i-1][m_up]+cnt); 
      } while (best < (current_best+cnt) && ((m_up = (m_up - im)&im)));
      dp[i][m] = best;
      current_best = max(current_best,dp[i][m]);
    }
  }

  cout << current_best << endl;
}

int main(){
  ios_base::sync_with_stdio(false);
  int T;
  cin >> T;
  while(T--) testcase();
}

