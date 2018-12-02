#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <bitset>
#include <unordered_map>

using namespace std;

int n;
vector<int> m;
vector<vector<int>> c;
int max_m;

// <stacks,best_value>
unordered_map<int,int> DP;
int v_to_int(const vector<int>& x){
  int a = 0;
  int b = 1;
  for(int i=0;i<x.size();++i){
    a += b * x[i];
    b *= (max_m+1);
  }
  return a;
}

int solve(const vector<int>& x){
  int a = v_to_int(x);
  if(a==0) return 0;
  auto it = DP.find(a);
  if(it != DP.end()) return it->second;

  int best = -1;
  for(int i=1;i< (1<<n); ++i){
    bool valid=true;
    // check that every stack has at least 1 chip and that
    // the chip matches the others
    int chip = c[__builtin_ctz(i)][x[__builtin_ctz(i)]-1];
    for(int j=0;j<n;++j){
      if((i & (1<<j)) && (x[j]==0 || c[j][x[j]-1]!=chip)){
        valid = false;
        break;
      }
    } 
    if(!valid) continue;

    // create a new stack from the old ones without
    // the cheaps removed
    vector<int> y(x);
    for(int j=0;j<n;++j){
      if(i&(1<<j)){
        --y[j];
      }
    }
    int current = solve(y);
    int k = __builtin_popcount(i);
    if(k>1)
      current += 1<<(k-2);

    best = max(best,current);
  }

  DP.insert(make_pair(a,best));

  return best;
}

void testcase(){
  cin >> n;
  m.clear();
  m.resize(n,0);
  max_m=0;
  for(int i=0;i<n;++i){
    cin >> m[i];
    max_m = max(max_m,m[i]);
  }
  DP.clear();
  c.clear();
  c.resize(n,vector<int>(max_m,-1));
  for(int i=0;i<n;++i){
    for(int j=0;j<m[i];++j){
      cin >> c[i][j];
    }
  }

  int p = solve(m);
  cout << p << endl;

}

int main(){
  ios_base::sync_with_stdio(false);
  int T;
  cin >> T;
  while(T--) testcase();
}
