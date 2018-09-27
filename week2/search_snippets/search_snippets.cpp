#include <iostream>
#include <vector>
#include <set>
#include <limits>

using namespace std;

void testcase(){
  int n; cin >> n;
  vector<int> m(n,0);
  int tot_w = 0;
  for(int i=0;i<n;i++){
    int mi; cin >> mi;
    m[i] = mi;
    tot_w += mi;
  }


  set<pair<int,int>> poss;
  for(int i=0;i<n;i++){
    for(int j=0;j<m[i];j++){
      int pj; cin >> pj;
      poss.insert({pj,i});
    }
  }

  vector<pair<int,int>> pos(poss.begin(),poss.end());

  vector<int> count(n,0);
  set<int> missing;

  for(int i=0;i<n;i++) missing.insert(i);
  count[pos[0].second] = 1;

  int l = 0;
  int r = 0;
  int best_d=numeric_limits<int>::max();
  while(l<tot_w && r<tot_w){
    if(missing.size()>0){
      r++;
      int ty = pos[r].second;
      count[ty]++;
      if(missing.find(ty) != missing.end()){
        missing.erase(ty);
      }
    }else{
      int d = pos[r].first - pos[l].first+1;
      if(d < best_d) {
        best_d = d;
      }
      int ty = pos[l].second;
      count[ty]--;
      if(count[ty]==0){
        missing.insert(ty);
      }
      l++;
    }

  }
  cout << best_d << endl;
}

int main(){
  int t; cin >> t;
  for(int i=0;i<t;i++) testcase();
}
