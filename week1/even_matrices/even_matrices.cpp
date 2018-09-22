#include <iostream>
#include <vector>
#include <array>

using namespace std;
typedef vector<int> VI;

void testcase(){
  int n; cin >> n;

  vector<VI> bits(n+1,VI(n+1,0));
  vector<VI> sums(n+1,VI(n+1,0));

  for(int i=1;i<=n;i++){
    for(int j=1;j<=n;j++){
      int b; cin >> b;
      bits[i][j]=b;
      sums[i][j]= sums[i-1][j] + sums[i][j-1] - sums[i-1][j-1] + bits[i][j];
    }
  }

  int even_pairs=0;
  vector<vector<VI>> dp_e(n+1,vector<VI>(n+1,VI(n+1,0)));
  vector<vector<VI>> dp_o(n+1,vector<VI>(n+1,VI(n+1,0)));

  // init
  for(int r=1;r<=n;r++){
    for(int l=1;l<=r;l++){
      int s = sums[1][r]-sums[1][l-1];
      if(s%2==0){
        dp_e[1][l][r] = 1;
        even_pairs++;
      }
      else {
        dp_o[1][l][r] = 1;
      }
    }
  }

  for(int i=2;i<=n;i++){
    for(int r=1;r<=n;r++){
      for(int l=1;l<=r;l++){
        int s = sums[i][r]-sums[i][l-1]-sums[i-1][r]+sums[i-1][l-1];
        if(s%2==0) {
          dp_e[i][l][r] = dp_e[i-1][l][r]+1;
          dp_o[i][l][r] = dp_o[i-1][l][r];
        }
        else {
          dp_o[i][l][r] = dp_e[i-1][l][r]+1;
          dp_e[i][l][r] = dp_o[i-1][l][r];
        }
        even_pairs += dp_e[i][l][r];
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
