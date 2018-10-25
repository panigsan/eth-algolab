#include <iostream>
#include <vector>
#include <algorithm>
#include <bitset>
#include <map>
#include <utility>

using namespace std;

// Used for debugging
#define step(x)   //cout << "Step " << x << endl
#define trace(x)  //cout << #x << " = " << x << endl
#define btrace(x) //cout << #x << " = " << bitset<3>(x) << endl


void testcase(){
  int n,k;
  cin >> n >> k;

  vector<long> s(n);
  for(int i=0;i<n;i++){
    cin >> s[i];
  }

  // Subset sub of L1 and L2 sets
  vector<long> L1(1<<(n/2),0);
  vector<long> L2(1<<((n+1)/2),0);

  step("Computing subsets sum of L1 and L2");
  for(int i=0;i<n/2;++i){
    for(int j=0;j < (1<<i); ++j){
      L1[j | (1<<i)] = s[i]+L1[j];
    }
  }

  for(int i=0;i<(n+1)/2;++i){
    for(int j=0; j<(1<<i);++j){
      L2[j | (1<<i)] = s[i+n/2] + L2[j];
    }
  }

  step("Computing all Red and Blue team combinations in L2");
  // < <Blu - Red, #bits> , #combinations>
  map<pair<long,int>,long> ML2;
  for(int m_in=0; m_in < 1<<((n+1)/2); ++m_in){
    int tot_sum = L2[m_in];
    int m_red=0;
    int bits = __builtin_popcount(m_in);

    do{ // Trick to go through all subsets of m_in
      int m_blu = (~m_red) & m_in;
      pair<long,int> key = make_pair(L2[m_blu]-L2[m_red],bits);
      auto it = ML2.find(key);
      if(it == ML2.end()){
        ML2.insert(make_pair(key,1));
      }else{
        (it->second)++;
      }
    } while ( (m_red = (m_red - m_in) & m_in));
  }

  long sol=0;
  // For each combination in L1, check how many valid combinations
  // are in L2
  step("Check L1 with L2");
  for(int m_in=0; m_in < 1<<n/2; ++m_in){
    int m_red=0;
    int bits_in = __builtin_popcount(m_in);

    do{ // Trick to go through all subsets of m_in
      int m_blu = (~m_red) & m_in;
      long goal = L1[m_red]-L1[m_blu];
      for(int bits=0;bits<=(n+1)/2;++bits){
        if(bits_in + bits < n-k) continue;

        auto it = ML2.find(make_pair(goal,bits));
        if(it != ML2.end()){
          sol += it->second;
        }
      }
    } while ( (m_red = (m_red - m_in) & m_in));
  }

  cout << sol << endl;
}

int main(){
  ios_base::sync_with_stdio(false);

  int T; cin >> T;
  while(T--) testcase();
}
