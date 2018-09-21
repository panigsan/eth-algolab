#include <iostream>
#include <vector>
using namespace std;


int nchooses4(int n){
  return n*(n-1)*(n-2)*(n-3)/24;
}

int nchooses2(int n){
  return n*(n-1)*2;
}

void testcase(){
  int n; cin >> n;


  int bits [n+1][n+1];
  int sums [n+1][n+1];
  for(int i=0;i<=n;i++){
    for(int j=0;j<=n;j++){
      bits[i][j] = 0;
      sums[i][j] = 0;
    }
  }

  for(int i=1;i<=n;i++){
    for(int j=1;j<=n;j++){
      int b; cin >> b;
      bits[i][j]=b;
      sums[i][j]= sums[i-1][j] + sums[i][j-1] - sums[i-1][j-1] + bits[i][j];
    }
  }

  /*/
  for(int i=0;i<=n;i++){
    for(int j=0;j<=n;j++){
      cout << sums[i][j];
    }
    cout << endl;
  }
  */

  int even_pairs=0;

  for(int i1=1;i1<=n;i1++){
    for(int j1=1;j1<=n;j1++){
      for(int i2=i1;i2<=n;i2++){
        for(int j2=j1;j2<=n;j2++){
          int s = sums[i2][j2] - sums[i2][j1-1] - sums[i1-1][j2] +
                  sums[i1-1][j1-1];
          if(s%2==0) even_pairs++;
        }
      }
    }
  }

  cout << even_pairs << endl;
}

int main(){
  ios_base::sync_with_stdio(false);
  int t; cin >> t;
  for(int i=0;i<t;i++){
    testcase();
  }
}
