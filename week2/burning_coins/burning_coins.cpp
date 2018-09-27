#include <iostream>
#include <vector>

using namespace std;
int solve(int,int,bool);

vector<int> v;
vector<vector<int>> dp_m;
vector<vector<int>> dp_o;
int n;
void testcase(){
  cin >> n;
  v.clear();
  dp_m.clear();
  dp_o.clear();
  for(int i=0;i<n;i++){
    dp_m.push_back(vector<int>(n,-1));
    dp_o.push_back(vector<int>(n,-1));
  }
  for(int i=0;i<n;i++){
    int vi; cin >> vi;
    v.push_back(vi);
  }

  int sol = solve(0,n-1,true);
  cout << sol << endl;

}

int solve(int l, int r, bool me){
  if(l>n || l>r) return -100000;
  if(l==r && me) return v[l];
  if(l==r && !me) return 0;
  //if(l+1==r && me) return max(v[l],v[r]);
  //if(l+1==r && !me) return min(v[l],v[r]);
  if(me && dp_m[l][r] != -1) return dp_m[l][r];
  if(!me && dp_o[l][r] != -1) return dp_o[l][r];

  int s;
  if(me){
    s = max(v[l]+solve(l+1,r,false), v[r]+solve(l,r-1,false));
    dp_m[l][r] = s;
  } else{
    s = min(solve(l+1,r,true), solve(l,r-1,true));
    dp_o[l][r] = s;
  }
  return s;
}

int main(){
  int t; cin >> t;
  for(int i=0;i<t;i++) testcase();
}
