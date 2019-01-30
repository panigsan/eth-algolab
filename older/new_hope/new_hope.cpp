#include <iostream>
#include <vector>
#include <cassert>
#include <algorithm>
#include <queue>
#include <bitset>

using namespace std;

#define trace(x) cerr << #x << " = " << x << endl
#define btrace(x) cerr << #x << " = " << bitset<3>(x) << endl

int k,s,m;
// edges between stormtroppers
vector<vector<int>> edges;
// edges between command centers
vector<vector<int>> edgesC;

// return a unique index of the storm in the command center (0..k*s)
int index(int command, int storm){
    return command*s + storm;    
}
// return the id of the command center where the storm belongs (0..k-1)
int commander(int index){
    return index/s;    
}
// return the storm id inside of the command center (0..s-1)
int restorm(int index){
    return index-s*commander(index);    
}

// dp[c][map] = maximum independent set from the c-th command center to the leaves
vector<vector<int>> dp;
// sub_dp[c][map] = maximum independent set of the c-th command center with map storm available
vector<vector<int>> sub_dp;

int solve(int com, int map){
    // if it is the last command center
    if(edgesC[com].size()==0) return sub_dp[com][map];
    if(dp[com][map]!=-1) return dp[com][map];
    int best=0;
    // for each next command center
    for(int next : edgesC[com]){
        // new_map stores which stormtroopers are available in the next command center
        // which depends on which stormtroopers are supervised by the current command
        // center
        int new_map = (1<<s)-1;
        for(int i=0;i<s;++i){
            // if stormtrooper i in current command center is available,
            // then all his children in the next command center are not available
            if(map & (1<<i)){
                // iterate over all his children
                for(int next_st : edges[index(com,i)]){
                    // if the children doesn't belong to the next command center, skip it
                    if(commander(next_st)!=next) continue;
                    // otherwise remove it from the available map
                    int next_st_id = restorm(next_st);
                    new_map &= ~(1<<next_st_id); 
                }
            }
        }
        // iterate over all sub mask for the next command center
        // to get the greatest value
        int current = 0;
        int sub_msk = 0;
        do {
            current = max(current,solve(next,sub_msk));
        } while ((sub_msk=(sub_msk-new_map)&new_map));
        best += current;
    }

    best += sub_dp[com][map];
    
    dp[com][map] = best;
    return best;
}

void testcase(){
    cin >> k >> s >> m;
    edges.clear();
    edges.resize(k*s,vector<int>());
    edgesC.clear();
    edgesC.resize(k,vector<int>());
    dp.clear();
    dp.resize(k,vector<int>(1<<s,-1));
    sub_dp.clear();
    sub_dp.resize(k,vector<int>(1<<s,-1));


    // create graph
    for(int i=0;i<m;++i){
        int u,v,h;
        cin >> u >> v >> h;
        // if a stormtrooper watches someone in another command center,
        // then the command center u has an edge to the command center v
        if(u!=v) edgesC[u].push_back(v);
        for(int j=0;j<h;++j){
            int x,y;
            cin >> x >> y;
            // index(u,x) is the unique id of the x startropper in center u
            edges[index(u,x)].push_back(index(v,y));
        }
    }

    // compute all maximum independent set for each command center
    for(int c=0;c<k;++c){
        // try all mappings
        for(int i=0;i<(1<<s);++i){
            // valid is true if no two stormtroopers are watching each other
            bool valid=true;
            // check that no two troops are supervising eachother
            for(int storm=0;storm<s;++storm){
                if(i & (1<<storm)){
                    // storm is not allowed to supervise any other in the same command center
                    // which is also mapped
                    for(int next : edges[index(c,storm)]){
                        if(commander(next)!=c) continue;
                        if(i & (1<<restorm(next))){
                            valid = false;
                            break;
                        }
                    } 
                    if(!valid) break;
                }
            }     
            int count = 0;
            if(!valid){
                // if not valid, get the best from all submasks
                int sub_msk = 0;
                do{
                    count = max(count,sub_dp[c][sub_msk]); 
                } while((sub_msk=(sub_msk-i)&i));
            } else {
                count = __builtin_popcount(i);    
            }
            sub_dp[c][i]=count;
        }    
    }

    int best=0;
    for(int i=0;i<(1<<s);++i){
        best = max(best,solve(0,i));
    }
    cout << best << endl;

}

int main(){
    ios_base::sync_with_stdio(false);
    s = 3;
    int T;
    cin >> T;
    while(T--) testcase();
}
