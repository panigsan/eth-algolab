#include <iostream>
#include <vector>
#include <set>
#include <limits>
#include <algorithm>

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


  vector<vector<int>> p;
  set<pair<int,int>> data;
  for(int i=0;i<n;i++){
    p.push_back(vector<int>());
    for(int j=0;j<m[i];j++){
      int pj; cin >> pj;
      data.insert({pj,i});
      p[i].push_back(pj);
    }
  }

  vector<int> pos;
  vector<int> typs;
  for(pair<int,int> d : data){
    pos.push_back(d.first);
    typs.push_back(d.second);
  }


  vector<int> count(n,0);
  set<int> missing;
  int l=0;
  int r=0;
  int best_d = numeric_limits<int>::max();
  for(int i=0;i<n;i++) missing.insert(i);
  auto it = missing.find(typs[0]);
  missing.erase(it);
  count[typs[0]] = 1;

  while(l<tot_w && r<tot_w){
    if(missing.size()>0){
      r++;
      if(r>=tot_w) break;
      int ty = typs[r];
      count[ty]++;
      auto it = missing.find(ty);
      if(missing.find(ty) != missing.end()){
        missing.erase(it);
      }
    }else{
      int d = pos[r] - pos[l]+1;
      if(d < best_d) {
        best_d = d;
      }
      int ty = typs[l];
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
