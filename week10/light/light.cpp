#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <iostream>

#define trace(x) std::cerr << #x << " = " << x << std::endl

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_with_info_2<int,K> Vb;
typedef CGAL::Triangulation_face_base_2<K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds> Triangulation;
typedef K::Point_2 P;
typedef K::Circle_2 C;

void testcase(){
  int m,n;
  std::cin >> m >> n;
  std::vector<std::pair<P,long>> pts(m);
  std::vector<int> killed_by(m,-1);
  for(int i=0;i<m;++i){
    long x,y,r;
    std::cin >> x >> y >> r;
    pts[i].first = P(x,y);
    pts[i].second = r;
  }
  int h;
  std::cin >> h;

  std::vector<P> lights(n);
  for(int i=0;i<n;++i){
    std::cin >> lights[i];
  }
  Triangulation t;
  /*
  t.insert(lights.begin(),lights.end());
  */
  for(int i=0;i<n;++i){
    auto v = t.insert(lights[i]);
    v->info() = i;
  }

  int killed=0;
  int last_killer=-1;
  for(int i=0;i<m;++i){
    auto v = t.nearest_vertex(pts[i].first);
    long d = (h+pts[i].second);
    d = d*d;
    if(CGAL::squared_distance(v->point(),pts[i].first)<d){
      killed_by[i] = v->info();
      last_killer = std::max(last_killer,killed_by[i]);
      ++killed;
    }
  }

  if(killed<m){
    for(int i=0;i<m;++i){
      if(killed_by[i]==-1) std::cout << i << " ";
    }
    std::cout << std::endl;
  } else {
    for(int i=0;i<m;++i){
      if(killed_by[i] == last_killer) std::cout << i << " ";
    }
    std::cout << std::endl;
  }

}

int main(){
  std::ios_base::sync_with_stdio(false);
  int T;
  std::cin >> T;
  while(T--) testcase();
}
