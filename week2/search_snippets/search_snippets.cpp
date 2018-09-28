#include <iostream>
#include <vector>
#include <set>
#include <limits>
#include <algorithm>
#include <cassert>

using namespace std;

void testcase(){
  int n; cin >> n;
  vector<int> m(n,0);
  for(int i=0;i<n;i++){
    int mi; cin >> mi;
    m[i]=mi;
  }
  vector<vector<int>> p(n,vector<int>());
  for(int i=0;i<n;i++){
    for(int j=0;j<m[i];j++){
      int pj; cin >> pj;
      p[i].push_back(pj);
    }
  }
  // {index,type} from where I take the words
  set<pair<int,int>> pos;

  // at the beginning I take for each word their first occurence
  for(int i=0;i<n;i++) pos.insert({p[i][0],i});

  int size = numeric_limits<int>::max();
  while(1){
    pair<int,int> smallest = *pos.begin();
    pair<int,int> largest = *pos.rbegin();
    int s = largest.first - smallest.first+1;
    if(s<size) size=s;

    // remove the word at the smallest index and find the next one
    int index = smallest.first;
    int type = smallest.second;
    auto it = upper_bound(p[type].begin(),p[type].end(),index);
    if(it==p[type].end()) break;

    int new_index = *it;
    pos.erase(pos.begin());
    pos.insert({new_index,type});
  }

  cout << size << endl;
}

int main(){
  ios_base::sync_with_stdio(false);
  int t; cin >> t;
  for(int i=0;i<t;i++) testcase();
}
