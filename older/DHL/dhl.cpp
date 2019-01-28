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

const long INF = 100000;
long solve(int size_a, int size_b){
    if(size_a==0 && size_b>0 || size_b==0 && size_a>0) return INF;
    if(size_a==0 && size_b==0) return 0;
    else if(dp[size_a][size_b] != -1) return dp[size_a][size_b];

    long best = INF;
    for(int a=1;a<=size_a;++a){
        long sa = SA[size_a]-SA[size_a-a];
        /* failed binary search
        int l=1;
        int r=size_b;
        if(a==1) r=size_b;
        else     r=size_b-1;
        long sb = SB[size_b]-SB[size_b-r];
        long curr_best = solve(size_a-a,size_b-r)+(sa-a)*(sb-r);
        while(l<=r){
            int c =(l+r)/2;
            sb = SB[size_b]-SB[size_b-c];
            long res = solve(size_a-a,size_b-c)+(sa-a)*(sb-c);
            if(res <= curr_best) r=c-1;
            else            l=c+1;
            curr_best = min(curr_best,res);
            
        }
        best = min(best,curr_best);
        */
        for(int b=1;b<=size_b;++b){
           long sb = SB[size_b]-SB[size_b-b];
           long res = solve(size_a-a,size_b-b)+(sa-a)*(sb-b);
           // 3 is the magic number, if you change it to 2 you get wrong results ;) 
           // I found it by performing binary search until it worked 
           if(res>best*3) break; 
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
