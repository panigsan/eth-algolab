#include <iostream>
#include <limits>
#include <vector>
#include <algorithm>

using namespace std;

#define trace(x) cout << #x << " = " << x << endl

void testcase(){
  int n; cin >> n;
  vector<int> p(n);
  vector<int> l(n);

  vector<pair<int,int>> data;
  for(int i=0;i<n;i++){
    int li, pi; cin >> li >> pi;
    data.push_back(make_pair(pi,li));
  }
  sort(data.begin(),data.end());
  for(int i=0;i<n;i++){
    p[i] = data[i].first;
    l[i] = data[i].second;
  }


  int sol=1;
  int last = p[0];

  // Idea:
  // pick the one which ends first
  int i=1;
  while(i<n){
    int chosen_j=-1;
    int best = numeric_limits<int>::max();
    for(int j=i;j<n;j++){
      if(p[j]>=best) break;
      if(p[j]>=last){
        int cur = max(last+l[j],p[j]);
        if(cur < best){
          best = cur;
          chosen_j=j;
        }
      }
    }

    if(chosen_j==-1) break;
    last = best;
    sol++;
    i=chosen_j+1;
  }



  cout << sol << endl;
}

int main(){
  ios_base::sync_with_stdio(false);
  int T; cin >> T;
  while(T--) testcase();
}
