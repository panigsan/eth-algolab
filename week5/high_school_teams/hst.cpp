#include <iostream>
#include <vector>
#include <cassert>
#include <bitset>

using namespace std;

#define trace(x) cout << #x << " = " << x << endl
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

  int sol=0;
  for(int m_out=0;m_out< 1<<n;++m_out){ // all subsets for K
    int sk = 0;
    for(int j=0; j<n;++j){
      if(m_out & (1<<j)) ++sk;
    }
    if(sk>k) continue;

    int goal=0;
    for(int j=0;j<n;++j){
      if(! (m_out & (1<<j))) goal += s[j];
    }
    if(goal %2 == 1) continue; //if odd it is not possible to divide in two
    goal = goal / 2;
    
    for(int m_red =0; m_red < 1<<n;++m_red){
      if(m_out & m_red) continue;
      int temp = 0;
      for(int b=0;b<n;++b){
        if(m_red & (1<<b)){
          temp += s[b];
        }
      }
      if(temp != goal) continue;
      if(goal != 0 && temp==goal){
        ++sol;
        continue;
      }

      int temp2 = 0;
      for(int b=0;b<n;++b){
        if(!(m_red& (1<<b)) && !(m_out&(1<<b))){
          temp2 += s[b];
        }
      }
      if(temp2==goal) ++sol;
    }
  }
  cout << sol << endl;
}

int main(){
  ios_base::sync_with_stdio(false);
  int T; cin >> T;
  while(T--) testcase();
}
