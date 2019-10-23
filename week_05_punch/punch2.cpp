#include <iostream>
#include <vector>

using namespace std;

#define trace(x) cout << #x << ":" << x << endl

pair<int,int> best(const pair<int,int> a, const pair<int,int> b){
    if(a.first < b.first) return a;
    if(a.first == b.first && a.second >= b.second) return a;
    return b;
}

void testcase(){
    int n,k;
    cin >> n >> k;

    vector<int> costs(n+1, 0);
    vector<int> volus(n+1, 0);
    for(int i=1; i<=n; ++i){
        cin >> costs[i];
        cin >> volus[i];
    }

    vector<vector<pair<int,int>>> dp(n+2, vector<pair<int,int>>(k+1, {1<<20,0}));
    dp[1][0] = {0,0};

    for(int i=1; i<=n; ++i){
        for(int v=0; v<=k; ++v){
            int next_vol = min(k, v + volus[i]);
            auto x = dp[i][v];
            dp[i+1][v] = best(dp[i+1][v], x);

            x.first += costs[i];
            dp[i][next_vol] = best(dp[i][next_vol], x);

            x.second++;
            dp[i+1][next_vol] = best(dp[i+1][next_vol], x);
        }
    }

    cout << dp[n+1][k].first << " " << dp[n+1][k].second << endl;
    
}

int main(){
    ios_base::sync_with_stdio(false);
    int t;
    cin >> t;
    while(t--) testcase();
}
