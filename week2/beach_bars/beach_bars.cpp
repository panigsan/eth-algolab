#include <iostream>
#include <vector>
#include <limits>
#include <algorithm>
#include <cassert>

using namespace std;

void testcase(){
  int n; cin >> n;
  vector<int> x(n,0);
  for(int i=0;i<n;i++){
    int pos; cin >> pos;
    x[i] = pos;
  }
  sort(x.begin(),x.end());

  int l=0;
  int s=1;
  vector<int> sol;
  int best_d = numeric_limits<int>::max();
  while(l+s<=n){
    int m = (x[l]+x[l+s-1])/2;
    int d = max(abs(m-x[l]),abs(x[l+s-1]-m));
    bool addl=false;
    bool addr=false;
    // check if you can add it on the left
    {
      int ml = m-1;
      int dl = max(abs(ml-x[l]),abs(x[l+s-1]-ml));
      if(dl==d) addl=true;
      if(dl<d){
        m=m-1;
        d=dl;
      }
    }
    // check if you can add it on the right
    {
      int mm = m+1;
      int dd = max(abs(mm-x[l]),abs(x[l+s-1]-mm));
      if(dd==d) addr=true;
      if(dd<d) {
        m = m+1;
        d = dd;
      }
    }
    if(d==best_d){
      sol.push_back(m);
      if(addl) sol.push_back(m-1);
      if(addr) sol.push_back(m+1);
    }else if(d<best_d){
      best_d = d;
      sol.clear();
      sol.push_back(m);
      if(addl) sol.push_back(m-1);
      if(addr) sol.push_back(m+1);
    }
    if(l+s+1<=n && x[l+s]-x[l]<=200){
      s++;
      best_d = numeric_limits<int>::max();
    }
    else l++;
  }
  sort(sol.begin(),sol.end());
  cout << s << ' ' << best_d << endl;
  for(int s : sol) cout << s << ' ';
  cout << endl;
}

int main(){
  int t; cin >> t;
  for(int i=0;i<t;i++) testcase();
}
