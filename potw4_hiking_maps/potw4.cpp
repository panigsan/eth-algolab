#include <iostream>
#include <algorithm>
#include <vector>
#include <unordered_set>
#include <set>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef K::Point_2 P;
typedef K::Line_2 L;
typedef K::Segment_2 S;
typedef K::Triangle_2 T;
using namespace std;

void testcase(){
  int m,n; cin >> m >> n;

  vector<P> points;
  vector<T> trigs;

  for(int i=0;i<m;i++){
    int x, y; cin >> x >> y;
    points.push_back(P(x,y));
  }

  for(int i=0;i<n;i++){
    vector<P> q;
    for(int j=0;j<6;j++){
      int q0,q1;
      cin >> q0 >> q1;
      q.push_back(P(q0,q1));
    }

    L l1(q[0],q[1]);
    L l2(q[2],q[3]);
    L l3(q[4],q[5]);

    auto o1 = CGAL::intersection(l1,l2);
    const P p1 = *(boost::get<P>(&*o1));
    auto o2 = CGAL::intersection(l2,l3);
    const P p2 = *(boost::get<P>(&*o2));
    auto o3 = CGAL::intersection(l3,l1);
    const P p3 = *(boost::get<P>(&*o3));

    trigs.push_back(T(p1,p2,p3));
  }

  vector<vector<int>> cont_leg(n,vector<int>());
  vector<vector<int>> pos(m-1,vector<int>());

  for(int i=0;i<n;i++){
    for(int j=0;j<m-1;j++){
      auto b1 = trigs[i].bounded_side(points[j]);
      auto b2 = trigs[i].bounded_side(points[j+1]);
      if((b1 == CGAL::ON_BOUNDED_SIDE || b1==CGAL::ON_BOUNDARY) &&
         (b2 == CGAL::ON_BOUNDED_SIDE || b2==CGAL::ON_BOUNDARY)){
        //cout << i << " has " << j << endl;
        cont_leg[i].push_back(j);
        pos[j].push_back(i);
      }else {
        //cout << i << " has not " << j << endl;
      
      }
    }
  }

  /*
  for(int i=0;i<n;i++){
    cout << "i: " << i << ": " << cont_leg[i].size() << endl;
    for(int x : cont_leg[i]) cout << x << " ";
    cout << endl;
  }*/

  int best = 10000;

  /*
  set<pair<int,int>> idx;
  for(int i=0;i<m-1;i++){
    idx.insert(make_pair(pos[i][0],i));
  }

  while(1){
    pair<int,int> minp = *idx.begin();
    pair<int,int> maxp = *idx.rbegin();
    int dist = maxp.first - minp.first;
    if(dist < best) best = dist;
    
    auto it = upper_bound(pos[minp.second].begin(),pos[minp.second].end(),minp.first);
    if(it==pos[minp.second].end())break;
    idx.erase(idx.begin());
    idx.insert(make_pair(*it,minp.second));

  }
  best++;
  */

  /*
  
  for(int b=0;b<n;b++){
    for(int e=1;e<=n;e++){
      if(e-b >= best) continue;
      vector<bool> missing(m-1,true);
      for(int i=b;i<e;i++){
        for(int x : cont_leg[i]) missing[x] = false;
      }
      
      bool good = true;
      for(int x =0;x<m-1;x++){
        if(missing[x]) good=false;
      }
      
      if(good && e-b < best)
        best = e-b;
    }
  }

  */
  unordered_set<int> missing;
  for(int i=0;i<m-1;i++) missing.insert(i);
  vector<int> cnt(m,0);
  int b=0,e=0;
  
  while(e<n){
    if(missing.size()>0){
      for(int x : cont_leg[e]){
        if(cnt[x]==0) missing.erase(missing.find(x));
        cnt[x]++;
      }
      e++;
    } else {
      if(e-b < best){
        best = e-b;
        //cout << "new best " << b << " - " << e << endl;
      }
      for(int x : cont_leg[b]){
        cnt[x]--;
        if(cnt[x]==0) missing.insert(x);
      }
      b++;
    }
  }

  cout << best << endl;
}


int main(){
  ios_base::sync_with_stdio(false);
  int c; cin >> c;
  while(c--) testcase();
}
