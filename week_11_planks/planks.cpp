#include <iostream>
#include <numeric>
#include <algorithm>
#include <vector>
#include <utility>
#include <tuple>
#include <bitset>

using namespace std;

#define trace(x) //cerr << #x << " = " << x << endl
#define btrace(x) //cerr << #x << " = " << bitset<10>(x) << endl

typedef tuple<long,long,long,long> Four;

void testcase(){
  int n;
  cin >> n;
  int n1 = n/2;
  int n2 = (n+1)/2;
  vector<long> L1(n1);
  vector<long> L2(n2);
  for(int i=0;i<n1;++i) cin >> L1[i];
  for(int i=0;i<n2;++i) cin >> L2[i];

  long sum_l = accumulate(L1.begin(),L1.end(),(long)0);
  sum_l += accumulate(L2.begin(),L2.end(),(long)0);

  if(sum_l%4 != 0) {
    cout << "0" << endl;
    return;
  }
  long goal = sum_l/4;

  vector<Four> m_L2;
  for(long i=0;i<(1<<n2*2);++i){
    vector<long> u(4,0);

    for(int j=0;j<n2;++j){
      long idx = (i & (3<<2*j)) >> 2*j;
      u[idx] += L2[j];
    }

    if(u[0]<=goal && u[1]<=goal && u[2]<= goal && u[3]<=goal){
      m_L2.push_back(make_tuple(u[0],u[1],u[2],u[3]));
    }
  }

  sort(m_L2.begin(),m_L2.end());

  long sol=0;
  for(long i=0;i<(1<<n1*2);++i){
    vector<long> u(4,0);

    for(int j=0;j<n1;++j){
      long idx = (i & (3<<2*j)) >> 2*j;
      u[idx] += L1[j];
    }
    
    Four x = make_tuple(goal-u[0],goal-u[1],goal-u[2],goal-u[3]);
    auto bounds = equal_range(m_L2.begin(),m_L2.end(),x);
    sol += distance(bounds.first,bounds.second);
  }
  cout << sol/(long)24 << endl;
}

int main(){
  ios_base::sync_with_stdio(false);
  int T;
  cin >> T;
  while(T--) testcase();
}
