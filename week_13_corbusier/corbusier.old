#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
#include <unordered_set>

using namespace std;

#define trace(x) cerr << #x << " = " << x << endl

void testcase(){
  int n,I,k;
  cin >> n >> I >> k;
  trace(n);

  vector<int> h(n);
  for(int i=0;i<n;++i){
    long hi;
    cin >> hi;
    h[i] = (hi%k);
  }

  int split = (n+1)/2;
  unordered_set<int> left;
  for(int i=1; i< (1<<((n+1)/2));++i){
    int s=0;
    for(int j=0;j<split;++j){
      if(i&(1<<j)) s += h[j]%k;
    }
    s %= k;
    left.insert(s);
    if(s==I){
      cout << "yes" << endl;
      return;
    }
  }
  trace(left.size());

  unordered_set<int> right;
  for(int i=0;i<(1<<(n/2));++i){
    int s=0;
    for(int j=0;j<split;++j){
      if(i&(1<<j)) s += h[(n+1)/2+j];
    }
    s %= k;
    right.insert(s);

    auto it = left.find((I-s)%k);
    if(it!=left.end() || (i!=0 && s==I)){
      cout << "yes" << endl;
      return;
    }
  }
  
  cout << "no" << endl;

}

int main(){
  ios_base::sync_with_stdio(false);
  int T;
  cin >> T;
  while(T--) testcase();
}
