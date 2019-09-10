#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <iostream>
#include <algorithm>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K> Triangulation;
typedef K::Point_2 P;
typedef K::Circle_2 C;

void testcase(){
  int m,n;
  std::cin >> m >> n;
  // [<Point, radius>]
  std::vector<std::pair<P,long>> persons;
  for(int i=0;i<m;++i){
    long x,y,r;
    std::cin >> x >> y >> r;
    persons.push_back({P(x,y),r});
  }
  long h;
  std::cin >> h;

  std::vector<P> lights(n);
  for(int i=0;i<n;++i){
    std::cin >> lights[i];
  }
  Triangulation t;
  t.insert(lights.begin(),lights.end());
  
  int last_killed=-1;
  bool survivors=false;
  std::vector<int> hit_by(m,-1);
  for(int i=0;i<m;++i){
    P person = persons[i].first;
    long radi = persons[i].second;
    auto nearest = t.nearest_vertex(person);
    // if survives, fine
    if(CGAL::square(h+radi) <= CGAL::squared_distance(nearest->point(),person)){
      survivors=true;
      continue;
    }
    // if there are survivors, no need to compute first hit
    if(survivors){
      hit_by[i]=0;
      continue;
    }
    // otherwise search for first light which hits the person
    for(int j=0;j<n;++j){
      if(CGAL::square(h+radi) >= CGAL::squared_distance(person,lights[j])){
        hit_by[i] = j;
        last_killed = std::max(last_killed,j);
        break;
      }
    }
  }

  // Print if survived or killed by last possible light
  for(int i=0;i<m;++i){
    if((survivors && hit_by[i] == -1) || 
       (!survivors && hit_by[i]==last_killed)) std::cout << i << " ";
  }
  std::cout << std::endl;

}

int main(){
  std::ios_base::sync_with_stdio(false);
  int T;
  std::cin >> T;
  while(T--) testcase();
}
