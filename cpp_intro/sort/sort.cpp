#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

void testcase(){
  int n; cin >> n;
  vector<int> v(n);
  for(int i=0;i<n;i++){
    int a; cin >> a;
    v[i] = a;
  }
  int x; cin >> x;
  if(x==0){
    sort(v.begin(),v.end());
  }else{
    sort(v.begin(),v.end(),greater<int>());
  }
  for(int x : v) cout << x << ' ';
  cout << endl;
}

int main(){
  int t; cin >> t;
  for(int i=0;i<t;i++) testcase();
}
