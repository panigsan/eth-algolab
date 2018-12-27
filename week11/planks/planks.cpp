#include <iostream>
#include <algorithm>
#include <vector>
#include <utility>
#include <tuple>
#include <set>

using namespace std;

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

  long suml1 = 0;
  for(int i=0;i<n1;++i) suml1 += L1[i];
  long suml2 = 0;
  for(int i=0;i<n2;++i) suml2 += L2[i];
  if((suml1+suml2)%4 != 0) {
    cout << "0" << endl;
    return;
  }
  long goal = (suml1+suml2)/4;

  vector<Four> m_L1;
  //vector<Four> m_L2;
  set<Four> m_L2;

  for(int i=0;i<(1<<n2*2);++i){
    long a=0;
    long b=0;
    long c=0;
    long d=0;

    for(int j=0;j<n2;++j){
      int idx = (i & (3<<2*j)) >> 2*j;
      if(idx==0) a += L2[j];
      else if(idx==1) b += L2[j];
      else if(idx==2) c += L2[j];
      else if(idx==3) d += L2[j];
    }

    if(a<=goal && b<=goal && c<= goal && d<=goal){
      //m_L2.push_back(make_tuple(a,b,c,d));
      m_L2.insert(make_tuple(a,b,c,d));
    }
  }


  //sort(m_L2.begin(),m_L2.end());

  for(int i=0;i<(1<<n1*2);++i){
    long a=0;
    long b=0;
    long c=0;
    long d=0;

    for(int j=0;j<n1;++j){
      int idx = (i & (3<<2*j)) >> 2*j;
      if(idx==0) a += L1[j];
      else if(idx==1) b += L1[j];
      else if(idx==2) c += L1[j];
      else if(idx==3) d += L1[j];
    }
    
    Four x = make_tuple(goal-a,goal-b,goal-c,goal-d);
    if(m_L2.count(x)>0){
      cout << 1 << endl;
      return;
    }
  }

  /*
  for(auto x : m_L2){
    cout << get<0>(x) << " " << get<1>(x) << " " << get<2>(x) << " " << get<3>(x) << endl;
  }
  */

  cout << 0 << endl;


}

int main(){
  ios_base::sync_with_stdio(false);
  int T;
  cin >> T;
  while(T--) testcase();
}
