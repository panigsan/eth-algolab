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
vector<vector<int>> edges;
vector<vector<int>> edgesC;
int index(int command, int storm){
    return command*s + storm;    
}
int commander(int index){
    return index/s;    
}
int restorm(int index){
    return index-s*commander(index);    
}

/*
pair<int,int> solve(int idx){
    if(edges[idx].size()==0) return {0,1};

    pair<int,int> current = {0,1};
    for(int next : edges[idx]){
        pair<int,int> next_res = solve(next);
        current.first += max(next_res.first,next_res.second);
        current.second += next_res.first;
    }
    return current;
}
*/

vector<vector<int>> dp;
vector<vector<int>> sub_dp;

int solve(int com, int map){
    if(edgesC[com].size()==0) return sub_dp[com][map];
    if(dp[com][map]!=-1) return dp[com][map];
    int best=0;
    for(int next : edgesC[com]){
        int new_map = (1<<s)-1;
        for(int i=0;i<s;++i){
            if(map & (1<<i)){
                for(int next_st : edges[index(com,i)]){
                    if(commander(next_st)!=next) continue;
                    int next_st_id = restorm(next_st);
                    new_map &= ~(1<<next_st_id); 
                }
            }
        }
        best += solve(next,new_map);
    }

    best += sub_dp[com][map];
    
    dp[com][map] = best;
    return best;
}

void testcase(){
    cin >> k >> s >> m;
    // edges between stormtroppers
    edges.clear();
    edges.resize(k*s,vector<int>());
    // edges between command centers
    edgesC.clear();
    edgesC.resize(k,vector<int>());
    dp.clear();
    dp.resize(k,vector<int>(1<<s,-1));
    // sub_dp[k][map] = maximum points in center k with the mapping
    sub_dp.clear();
    sub_dp.resize(k,vector<int>(1<<s,-1));


    // create graph
    for(int i=0;i<m;++i){
        int u,v,h;
        cin >> u >> v >> h;
        // if a stormtrooper watches someone in another
        if(u!=v) edgesC[u].push_back(v);
        for(int j=0;j<h;++j){
            int x,y;
            cin >> x >> y;
            // index(u,x) is the unique id of the x startropper in center u
            edges[index(u,x)].push_back(index(v,y));
        }
    }

    // fill all possibilities for each command center
    for(int c=0;c<k;++c){
        trace(c);
        // try all mappings
        for(int i=0;i<(1<<s);++i){
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
            btrace(i);
            trace(count);
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
