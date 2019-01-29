#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

void testcase(){
    int n;
    cin >> n;

    vector<vector<long>> dp(n+1,vector<long>(n+1,0));
    vector<long> A(n+1,0);
    vector<long> B(n+1,0);

    vector<long> SA(n+1,0);
    vector<long> SB(n+1,0);
    for(int i=1;i<=n;++i){
        cin >> A[i];
        --A[i];
    }
    for(int i=1;i<=n;++i){
        cin >> B[i];
        --B[i];
    }

    for(int i=1;i<=n;++i) SA[i] = A[i]+SA[i-1];
    for(int i=1;i<=n;++i) SB[i] = B[i]+SB[i-1];

    for(int i=1;i<=n;++i) dp[1][i] = A[1]*SB[i];
    for(int i=1;i<=n;++i) dp[i][1] = B[1]*SA[i];

    for(int i=2;i<=n;++i){
        for(int j=2;j<=n;++j){
            dp[i][j] = A[i]*B[j]+min(dp[i-1][j-1], min(dp[i-1][j],dp[i][j-1]));
        }    
    }

    cout << dp[n][n] << endl;
}

int main(){
    ios_base::sync_with_stdio(false);
    int T;
    cin >> T;
    while(T--) testcase();
}
