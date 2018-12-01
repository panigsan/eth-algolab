#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include <set>

using namespace std;

#define step(x) //cerr << "Step " << x << endl

void testcase(){
  int n,q;
  cin >> n >> q;

  unordered_map<string,int> names;
  vector<string> inames(n);
  vector<int> parents(n,-1);
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

  set<int> S;
  for(int i=0;i<q;++i){
    string s;
    int b;
    cin >> s >> b;
    int u = names.find(s)->second;
    
    assert(S.find(u) == S.end());

    while(parents[u] != -1 && ages[parents[u]] <= b) u = parents[u];

    cout << inames[u] << " ";
  }

  cout << endl;

}


int main(){
  ios_base::sync_with_stdio(false);
  int T;
  cin >> T;
  while(T--) testcase();
}
