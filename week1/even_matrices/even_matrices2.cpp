#include <vector>
#include <iostream>

using namespace std;

void testcase(){
  int n;
  cin >> n;

  vector<vector<int>> X(n+1,vector<int>(n+1));
  vector<vector<int>> A(n+1,vector<int>(n+1,0));
  for(int i=1;i<=n;++i)
    for(int j=1;j<=n;++j)
      cin >> X[i][j];

  for(int i=1;i<=n;++i)
    for(int j=1;j<=n;++j)
      A[i][j] = A[i-1][j]+A[i][j-1]-A[i-1][j-1]+X[i][j];

  int tot=0;
  for(int i1=1;i1<=n;++i1){
    for(int i2=i1;i2<=n;++i2){
      int even = 0;
      int odd = 0;
      int q = 0;
      for(int j=1;j<=n;++j){
        int t = A[i2][j]-A[i2][j-1]-A[i1-1][j]+A[i1-1][j-1];
        q += t;
        if(q%2==0) ++even;
        else       ++odd;
      }
      tot += even * (even-1)/2 + odd * (odd-1)/2 + even;
    }
  }

  cout << tot << endl;
}
int main(){
  ios_base::sync_with_stdio(false);
  int T;
  cin >> T;
  while(T--) testcase();
}
