#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
#include <cmath>

using namespace std;

#define trace(x) cout << #x << " = " << x << endl

void testcase(){
  int n; cin >> n;
  vector<int> t(n);
  vector<bool> alive(n,true);
  vector<pair<int,int>> m(n,{1<<22,1<<22});
  set<int> s;
  for(int i=0;i<n;++i) {
    cin >> t[i];
    s.insert(t[i]);
  }

  vector<int> dict;
  for(int x : s) dict.push_back(x);

  for(int i=0;i<n;++i){
    int priority = find(dict.begin(),dict.end(),t[i]) - dict.begin();
    m[i].first = priority+1;
  }

  for(int j=(n-1)/2;j<n;++j){
    m[j].second = m[j].first;
  }
  for(int j=(n-3)/2;j>=0;--j){
    if(2*j+2<n){
      m[j].second = min(m[j].second, m[2*j+1].second);
      m[j].second = min(m[j].second, m[2*j+2].second);
    } else {
      m[j].second = m[j].first;
    }
  }
  /*
  for(int i=0;i<n;++i) {
    trace(t[i]);
    trace(m[i].first);
    trace(m[i].second);
  }
  */

  bool failed=false;
  int time=1;
  while(time<=n){
    int goal = m[0].second;

    int i=0;

    while(2*i+2<n && (alive[2*i+1] || alive[2*i+2])){
      if(alive[2*i+1] && goal == m[2*i+1].second){
        if(goal == m[2*i+1].first) goal = m[2*i+1].second;
        i = 2*i+1;
      } else {
        if(goal == m[2*i+2].first) goal = m[2*i+2].second;
        i = 2*i+2;
      }
    }
    trace(i);
    if(t[i]>time) {
      trace(t[i]);
      trace(time);
      failed = true;
      break;
    }

    alive[i] = false;
    int parent = i;
    while(parent!=0){
      parent = (parent-1)/2;
      if(!alive[2*parent+1] && alive[2*parent+1]){
        m[parent].second = m[parent.first];
      } else {
        if(alive[2*parent+1]){
          m[parent].second = m[2*parent+1].second;
        }

      }
      /*
      if(alive[2*parent+1] && alive[2*parent+1]){
        m[parent].second = min(m[2*parent+1].second,m[2*parent+2].second);
        m[parent].second = min(m[parent].second,m[2*parent+1].first);
        m[parent].second = min(m[parent].second,m[2*parent+2].first);
      }
      else if(alive[2*parent+1]){
        m[parent].second = min(m[2*parent+1].first,m[2*parent+1].second);
      } else if(alive[2*parent+2]){
        m[parent].second = min(m[2*parent+2].first,m[2*parent+2].second);
        //m[parent].second = m[2*parent+2].second;
      } else {
        m[parent].second = m[parent].first;
      }
      */
    }


    time++;
  }

  cout << (failed ? "no" : "yes") << endl;
}

int main(){
  ios_base::sync_with_stdio(false);
  int T; cin >> T;
  while(T--) testcase();
}
