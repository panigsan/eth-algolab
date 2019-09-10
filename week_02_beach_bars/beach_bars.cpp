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
    if(d==best_d){
      sol.push_back(m);
      if(max(m-1-x[l],x[l+s-1]-m+1)==d) sol.push_back(m-1);
      if(max(m+1-x[l],x[l+s-1]-m-1)==d) sol.push_back(m+1);
    }else if(d<best_d){
      best_d = d;
      sol.clear();
      sol.push_back(m);
      if(max(m-1-x[l],x[l+s-1]-m+1)==d) sol.push_back(m-1);
      if(max(m+1-x[l],x[l+s-1]-m-1)==d) sol.push_back(m+1);
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
