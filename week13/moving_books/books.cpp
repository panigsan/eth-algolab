
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

#define trace(x) std::cerr << #x << " = " << x << std::endl
void testcase() {
  int n,m;
  cin >> n >> m;

  vector<long> people(n);
  vector<long> books(m);
  for(int i=0;i<n;++i) cin >> people[i];
  for(int i=0;i<m;++i) cin >> books[i];

  sort(people.begin(),people.end());
  sort(books.begin(),books.end());

  if(*people.rbegin() < *books.rbegin()){
    cout << "impossible" << endl;
    return;
  }
  long l=1;
  long r=m;
  long c;
  while(l<r){
    c = (l+r)/2;

    int to_take=m-1;
    // for each person, try to take the largest books
    for(int p=n-1;p>=0;--p){
      int taken=0;
      while(to_take>=0 && taken<c && people[p]>=books[to_take]){
        ++taken;
        --to_take;
      }
    }

    // if one books has not been taken
    if(to_take>=0){
      l = c+1;  
    } else {
      r = c;  
    }
  }
  c=(l+r)/2;
  cout << (3*c-1) << endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int T;
  std::cin >> T;
  while(T--) testcase();
}
