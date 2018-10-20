#include <iostream>
#include <bitset>
#include <vector>
#include <map>

using namespace std;

#define trace(x) cout << #x << " = " << x << endl
#define btrace(x) cout << #x << " = " << bitset<18>(x) << endl

void proom(const vector<int> & v){
  for(int x : v) cout << x << " ";
  cout << endl;
}

// Idea:
// - Split the switches in two sets L1 and L2
// - Compute all combinations for L1 and L2
// - Store all mappings of L2 into a map <lights,n_bits>
// - For each L1, check if there is a valid combination in L2 
// The solution should run in O(2^(n/2) + 2^(n/2))
// - Building the DP and looking for valid matches
void testcase(){
  int N, M; cin >> N >> M;
  vector<int> b(M);
  vector<vector<int>> L_on(N,vector<int>(M,0));
  vector<vector<int>> L_off(N,vector<int>(M,0));
  for(int i=0;i<M;++i) cin >> b[i];

  for(int i=0;i<N;++i){
    for(int j=0;j<M;++j){
      cin >> L_on[i][j];
      cin >> L_off[i][j];
    }
  }

  vector<vector<int>> L1(1<<(N/2),vector<int>(M,0));
  vector<vector<int>> L2(1<<((N+1)/2),vector<int>(M,0));


  for(int i=0;i<N/2;++i){
    for(int j=0;j<M;++j){
      L1[0][j] += L_on[i][j];
    }
  }
  for(int i=N/2;i<N;++i){
    for(int j=0;j<M;++j){
      L2[0][j] += L_on[i][j];
    }
  }

  // precompute all possible combinations for L1 and L2 using DP
  // because we want it faaast
  for(int i=0;i<(N/2); ++i){
    for(int m_l1=0;m_l1 < 1<<i; ++m_l1){
      for(int j=0;j<M;++j){
        L1[m_l1 | (1<<i)][j] = L1[m_l1][j] + L_off[i][j] - L_on[i][j];
      }
    }
  }

  for(int i=0;i< (N+1)/2; ++i){
    for(int m_l2=0; m_l2 < 1<<i; ++m_l2){
      for(int j=0;j<M;++j){
        L2[(m_l2 | (1<<i))][j] = L2[m_l2][j] + L_off[i+N/2][j] - L_on[i+N/2][j];
      }
    }
  }

  // <lights,n_switches>
  map<vector<int>,int> ML2;
  for(int i=0;i< 1 << (N+1)/2; ++i){
    auto it = ML2.find(L2[i]);
    // either create a new map object or update it if smaller
    if(it == ML2.end()) {
      ML2.insert(make_pair(L2[i],__builtin_popcount(i)));
    } else {
      int bits = __builtin_popcount(i);
      if(bits < it->second){
        it->second = bits;
      }
    }
  }

  int best = 100;
  for(int i=0; i < 1<<(N/2);++i){
    vector<int> goal(b);
    for(int j=0;j<M;++j) goal[j] -= L1[i][j];
    auto it = ML2.find(goal);
    if(it != ML2.end()){
      best = min(best,__builtin_popcount(i) + (it->second));
    }
  }

  if(best == 100)
    cout << "impossible" << endl;
  else
    cout << best << endl;
}

int main(){
  ios_base::sync_with_stdio(false);
  int T; cin >> T;
  while(T--) testcase();
}
