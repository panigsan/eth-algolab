#include <iostream>
#include <bitset>
#include <vector>
#include <array>

using namespace std;

#define trace(x) //cout << #x << " = " << x << endl
#define btrace(x) //cout << #x << " = " << bitset<3>(x) << endl

void proom(vector<int> r){
  //for(int x : r) cout << x << " ";
  //cout << endl;

}

void testcase(){
  int N, M; cin >> N >> M;
  vector<int> b(M);
  //vector<array<int,15>> L_on(N);
  //vector<array<int,15>> L_off(N);
  int L_on[N][M];
  int L_off[N][M];
  //vector<vector<pair<int,int>>> L(N,array<pair<int,vector<pair<int,int>>(M));
  int room[1<<N][M];
  for(int i=0;i<M;++i) room[0][i] = 0;
  //vector<array<int,15>> room(1<<N);
  for(int i=0;i<M;++i) cin >> b[i];

  for(int i=0;i<N;++i){
    for(int j=0;j<M;++j){
      cin >> L_on[i][j];
      cin >> L_off[i][j];
      //cin >> L[i][j].first;
      //cin >> L[i][j].second;
      room[0][j] += L_on[i][j];
    }
  }
  //proom(room[0]);

  for(int n_switches=1;n_switches<N;++n_switches){
    for(int m_old=0;m_old < (1<<N); ++m_old){
      if(__builtin_popcount(m_old) != n_switches-1) continue;
      
      for(int id_switch=0;id_switch<N;++id_switch){
        if(m_old & (1<<id_switch)) continue;
        trace(id_switch);
        btrace(m_old);
        //proom(room[m_old]);
        //proom(room[m_old | (1<<id_switch)]);
        bool all_right = true;
        for(int i=0;i<M;++i){
          room[m_old | (1<<id_switch)][i] = room[m_old][i];
          room[m_old | (1<<id_switch)][i] -= L_on[id_switch][i];
          room[m_old | (1<<id_switch)][i] += L_off[id_switch][i];
          if(room[m_old | (1<<id_switch)][i] != b[i]) all_right = false;
        }
        //proom(room[m_old | (1<<id_switch)]);
        if(all_right) {
          cout << n_switches << endl;
          return;
        }
      }
    }
  }

  /*
  for(int i=0;i< (1<<N);++i){
    btrace(i);
    proom(room[i]);

  }
  */
  cout << "impossible" << endl;


}

int main(){
  ios_base::sync_with_stdio(false);
  int T; cin >> T;
  while(T--) testcase();
}
