#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <limits>

using namespace std;

#define trace(x) //cerr << #x << " = " << x << endl
#define step(x) //cerr << "Step " << x << endl
const int INF = numeric_limits<int>::max();

void testcase(){
  int n; cin >> n;
  vector<int> t(n);
  vector<bool> alive(n,true);
  vector<pair<int,int>> m(n,{INF,INF});
  for(int i=0;i<n;++i) {
    cin >> t[i];
  }

  // assign priorities to the bomb by their times
  vector<int> t2(t);
  sort(t2.begin(),t2.end());
  for(int i=0;i<n;++i){
    int priority = lower_bound(t2.begin(),t2.end(),t[i]) - t2.begin();
    m[i].first = priority;
  }

  for(int j=(n-1)/2;j<n;++j){
    m[j].second = m[j].first;
  }
  for(int j=(n-3)/2;j>=0;--j){
    m[j].second = min(m[j].second, m[2*j+1].first);
    m[j].second = min(m[j].second, m[2*j+1].second);
    m[j].second = min(m[j].second, m[2*j+2].first);
    m[j].second = min(m[j].second, m[2*j+2].second);
  }

  bool failed=false;
  for(int time=1;time<=n;++time){
    int goal = m[0].second;

    // index of the bomb to destroy
    int i=0;

    // find the bomb to deactivate
    // by going through the tree
    while(2*i+2<n && (alive[2*i+1] || alive[2*i+2])){
      auto l = m[2*i+1];
      auto r = m[2*i+2];
      if(alive[2*i+1] && (goal == l.first || goal==l.second)){
        if(goal == l.first) goal = l.second;
        i = 2*i+1;
      } else {
        if(goal == r.first) goal = r.second;
        i = 2*i+2;
      }
    }
    
    if(t[i]<time) {
      failed = true;
      break;
    }

    // deactivate the bomb :O , pretty easy
    alive[i] = false;

    // update the parents
    int parent = i;
    while(parent!=0){
      parent = (parent-1)/2;
      if(!alive[2*parent+1] && !alive[2*parent+2]){ // no children anymore
        m[parent].second = m[parent].first;
      } else { // either one or two children left
        m[parent].second = INF;
        if(alive[2*parent+1]){
          int m_l = min(m[2*parent+1].first , m[2*parent+1].second);
          m[parent].second = min(m[parent].second,m_l);
        }
        if(alive[2*parent+2]){
          int m_r = min(m[2*parent+2].first , m[2*parent+2].second);
          m[parent].second = min(m[parent].second,m_r);
        }
      }
    }
  }

  cout << (failed ? "no" : "yes") << endl;
}

int main(){
  ios_base::sync_with_stdio(false);
  int T; cin >> T;
  while(T--) testcase();
}
