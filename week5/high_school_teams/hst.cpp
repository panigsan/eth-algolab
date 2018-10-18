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
  for(int m_in=0;m_in < (1<<n); ++m_in){
    if(__builtin_popcount(m_in)<n-k) continue;

    int goal = 0;
    for(int i=0;i<n;++i){
      if(m_in & (1<<i)) goal += s[i];
    }
    if(goal %2 == 1) continue;

    int m_sub=0;
    do {
      int s1 = 0;
      int s2 = 0;
      for(int i=0;i<n;++i){
        if(m_sub & (1<<i)) s1 += s[i];
        else if( m_in & 1<<i) s2 += s[i];
      }

      if(s1 == goal / 2 && s2 == goal/2) ++sol;
    } while( (m_sub=(m_sub-m_in)&m_in) );
  }

  cout << sol << endl;
}

int main(){
  ios_base::sync_with_stdio(false);

  int T; cin >> T;
  while(T--) testcase();
}
