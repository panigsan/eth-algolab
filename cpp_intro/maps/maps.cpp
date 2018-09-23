#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

void testcase(){
  int q; cin >> q;
  
  map<string,vector<int>> dp;
  for(int i=0;i<q;i++){
    int a; cin >> a;
    string b; cin >> b;
    if(a==0){
      //dp[b].clear();
      auto it = dp.find(b);
      if(it != dp.end()){
        it->second.clear();
      }
    }
    else {
      dp[b].push_back(a);
    }
  }

  string s; cin >> s;

  auto it = dp.find(s);
  if(it != dp.end() && it->second.size()!=0){
    vector<int> vals = it->second;
    sort(vals.begin(),vals.end());
    for(int x : vals) cout << x << ' ';
    cout << endl;
  } else {
    cout << "Empty" << endl;
  }

}

int main(){
  int t; cin >> t;
  for(int i=0;i<t;i++) testcase();
}
