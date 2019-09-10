#include <iostream>
#include <vector>
#include <unordered_map>
#include <map>
#include <string>
#include <set>

using namespace std;

void testcase(){
  int n,q;
  cin >> n >> q;

  unordered_map<string,int> names;
  vector<string> inames(n);
  vector<int> parents(n,-1);
  vector<int> p2(n,-1); // parent which points much higher in the tree
  vector<int> ages(n);

  for(int i=0;i<n;++i){
    string s;
    int a;
    cin >> s >> a;
    names.insert(make_pair(s,i));
    inames[i] = s;
    ages[i] = a;
  }

  for(int i=0;i<n-1;++i){
    string c,p;
    cin >> c >> p;
    parents[names.find(c)->second] = names.find(p)->second;
  }

  for(int i=0;i<q;++i){
    string s;
    int b;
    cin >> s >> b;
    int u = names.find(s)->second;
    int v = u;
    
    while(parents[v] != -1 && ages[parents[v]] <= b){
      if(p2[v] != -1 && ages[p2[v]] <= b) v = p2[v];
      else v = parents[v];
    }

    if(p2[u] == -1) p2[u] = v;
    else if(ages[v] > ages[p2[u]] ) p2[u] = v;
    cout << inames[v] << " ";
  }
  cout << endl;
}

int main(){
  ios_base::sync_with_stdio(false);
  int T;
  cin >> T;
  while(T--) testcase();
}
