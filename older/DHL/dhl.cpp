#include <iostream>
#include <vector>
#include <algorithm>


using namespace std;

#define trace(x) cerr << #x << " = " << x << endl
int n;
vector<long> A;
vector<long> B;
vector<long> SA;
vector<long> SB;
vector<vector<long>> dp;

long solve(int size_a, int size_b){
    if(size_a==0 && size_b>0 || size_b==0 && size_a>0) return 1000000;
    if(size_a==0 && size_b==0) return 0;
    else if(dp[size_a][size_b] != -1) return dp[size_a][size_b];

    long best = numeric_limits<long>::max();
    for(int a=1;a<=size_a;++a){
        for(int b=1;b<=size_b;++b){
           long sa = SA[size_a]-SA[size_a-a];
           long sb = SB[size_b]-SB[size_b-b];
           long res = solve(size_a-a,size_b-b)+(sa-a)*(sb-b);
           best = min(best,res);
        }    
    }
    dp[size_a][size_b] = best;
    return best;    
}

void testcase(){
    cin >> n;

    dp.clear();
    dp.resize(n+1,vector<long>(n+1,-1));
    A.clear();
    B.clear();
    A.resize(n);
    B.resize(n);

    SA.clear();
    SB.clear();
    SA.resize(n+1,0);
    SB.resize(n+1,0);
    for(int i=0;i<n;++i) cin >> A[i];
    for(int i=0;i<n;++i) cin >> B[i];

    for(int i=1;i<=n;++i) SA[i] = A[i-1]+SA[i-1];
    for(int i=1;i<=n;++i) SB[i] = B[i-1]+SB[i-1];


    cout << solve(n,n) << endl;
}

int main(){
    ios_base::sync_with_stdio(false);
    int T;
    cin >> T;
    while(T--) testcase();
}
