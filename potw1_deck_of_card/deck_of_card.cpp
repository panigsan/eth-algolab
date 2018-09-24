#include <iostream>
#include <vector>
#include <set>
#include <limits>

using namespace std;

void testcase(){
  int n; cin >> n;
  int k; cin >> k;

  vector<int> v(n,0);
  for(int i=0;i<n;i++){
    int vi; cin >> vi;
    v[i] = vi;
  }

  int best = numeric_limits<int>::max();
  int best_i=-1;
  int best_j=-1;

  vector<int> sum(n,0);
  sum[0]=v[0];
  for(int i=1;i<n;i++){
    sum[i] = sum[i-1]+v[i];
  }

  int i=0;
  int j=0;
  while(i<n && j<n){
    if(i>j)j++;
    int s = sum[j]-sum[i]+v[i];
    if(abs(s-k)==0){
      best_i=i;
      best_j=j;
      break;
    }
    else if(abs(s-k)<best && s-k<0){
      best = abs(s-k);
      best_i=i;
      best_j=j;
      j++;
    }
    else if(abs(s-k)<best && s-k>0){
      best = abs(s-k);
      best_i=i;
      best_j=j;
      i++;
    }
    else if(s-k>0){
      i++;
    }
    else {
      j++;
    }
  }
  cout << best_i << ' ' << best_j << endl;
}

int main(){
  ios_base::sync_with_stdio(false);
  int t; cin >> t;
  for(int i=0;i<t;i++) testcase();
}
