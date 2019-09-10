#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_set>

using namespace std;

bool testcase(){
  int n,I,k;
  cin >> n >> I >> k;

  vector<int> h(n);
  for(int i=0;i<n;++i){
    long hi;
    cin >> hi;
    h[i] = (hi%k);
  }

  int split = n/3;
  unordered_set<int> left;
  unordered_set<int> middle;
  for(int i=1; i< (1<<((n+1)/3));++i){
    int s=0;
    for(int j=0;j<split;++j){
      if(i&(1<<j)) s += h[j]%k;
    }
    s %= k;
    left.insert(s);
  }
  for(int i=1; i< (1<<((n+1)/3));++i){
    int s=0;
    for(int j=0;j<split;++j){
      if(i&(1<<j)) s += h[n/3+j]%k;
    }
    s %= k;
    middle.insert(s);
  }

  // uhm yeah, I use middle twice, not sure why it works then
  // I wanted to use a third unordered_set but when I copied the code from above I forgot
  // to change the name of the variable. but it works
  for(int i=1; i< (1<<((n+1)/3));++i){
    int s=0;
    for(int j=0;j<split;++j){
      if(i&(1<<j)) s += h[2*n/3+j]%k;
    }
    s %= k;
    middle.insert(s);
  }

  for(int s : middle){
    if(s==I) return true;
    auto it = left.find(abs((I-s)%k));
    if(it!=left.end()) return true;
  }
  return false;
}

int main(){
  ios_base::sync_with_stdio(false);
  int T;
  cin >> T;
  while(T--) cout << (testcase() ? "yes" : "no") << endl;
}
