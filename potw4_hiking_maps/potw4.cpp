#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include <utility>
#include <unordered_set>
#include <set>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef K::Point_2 P;
typedef K::Line_2 L;
typedef K::Segment_2 S;
typedef K::Triangle_2 T;
using namespace std;

#define trace(x) cout << #x << " = " << x << endl

// if using a triangle
inline bool point_in_trig(const T& t, const P& p){
  auto b1 = t.bounded_side(p);
  return b1 == CGAL::ON_BOUNDED_SIDE || b1==CGAL::ON_BOUNDARY;
}

// if using corners of a trianle
inline bool point_in_trig(const tuple<P,P,P>& t, const P& p) {
  bool a = !CGAL::left_turn(get<0>(t),get<1>(t),p);
  bool b = !CGAL::left_turn(get<1>(t),get<2>(t),p);
  // this line helps going from 80 to 100 points
  if(a!=b) return false;
  bool c = !CGAL::left_turn(get<2>(t),get<0>(t),p);

  return b == c;
}

void testcase(){
  int m,n; cin >> m >> n;

  vector<P> points;
  //vector<T> trigs;
  vector<tuple<P,P,P>> trigs;

  for(int i=0;i<m;i++){
    int x, y; cin >> x >> y;
    points.push_back(P(x,y));
  }

  // create the triangles
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
    auto o2 = CGAL::intersection(l1,l3);
    const P p2 = *(boost::get<P>(&*o2));
    auto o3 = CGAL::intersection(l2,l3);
    const P p3 = *(boost::get<P>(&*o3));

    //trigs.push_back(T(p1,p2,p3));
    trigs.push_back(make_tuple(p1,p2,p3));
  }

  // pos[i] contains the triangles which cover leg i
  vector<queue<int>> pos(m-1,queue<int>());

  // check where the legs belong
  for(int i=0;i<n;i++){
    // keep the previous stored so that it is not computed twice
    bool previous = point_in_trig(trigs[i],points[0]);
    for(int j=0;j<m-1;j++){
      bool next = point_in_trig(trigs[i],points[j+1]);
      if(previous && next){
        pos[j].push(i);
      }
      previous = next;
    }
  }

  int best = 10000;

  // search snippet
  set<pair<int,int>> idx;
  for(int i=0;i<m-1;i++){
    idx.insert(make_pair(pos[i].front(),i));
    pos[i].pop();
  }

  // idea:
  // idx is an ordered set of tuples where the first entry
  // contain an index of the triangle and the second contains
  // the number of the leg
  // we shift the window by removing the most-left pair,
  // hence removing a leg, and finding the next idx for which 
  // the removed leg is available (stored in the stack pos)
  while(1){
    pair<int,int> minp = *idx.begin();
    pair<int,int> maxp = *idx.rbegin();
    int dist = maxp.first - minp.first+1;
    if(dist < best) best = dist;
    
    if(pos[minp.second].size()==0) break;
    int new_idx = pos[minp.second].front();
    pos[minp.second].pop();
    idx.erase(idx.begin());
    idx.insert(make_pair(new_idx,minp.second));
  }

  cout << best << endl;
}


int main(){
  ios_base::sync_with_stdio(false);
  int c; cin >> c;
  while(c--) testcase();
}
