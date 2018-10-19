#include <iostream>
#include <vector>
#include <cassert>
#include <bitset>
#include <cmath>

using namespace std;

#define trace(x)  cout << #x << " = " << x << endl
#define btrace(x) cout << #x << " = " << bitset<12>(x) << endl


int n,k;
vector<int> s;

void testcase(){
  cin >> n >> k;
  s.clear();
  s.resize(n);
  for(int i=0;i<n;i++){
    cin >> s[i];
  }

  int sol = 0;
  vector<int> dp(1<<n);
  for(int i=0;i<n;++i){
    for(int j=0;j < (1<<i); ++j){
      dp[j | (1<<i)] = s[i] + dp[j];
    }
  }

  for(int m_in=0;m_in< (1<<n); ++m_in){
    if(__builtin_popcount(m_in) < n-k) continue;

    int m_red=0;
    do{
      if(dp[m_red] == dp[(~m_red) & m_in]) ++sol; 
    } while ((m_red = (m_red - m_in) & m_in));
  
  }

  cout << sol << endl;
}

int main(){
  ios_base::sync_with_stdio(false);

  int T; cin >> T;
  while(T--) testcase();
}
