#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Voronoi_diagram_2.h>
#include <iomanip>
#include <iostream>

#define trace(x) std::cout << #x << " = " << x << std::endl
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Finite_faces_iterator  Face_iterator;

int main()
{
  std::ios_base::sync_with_stdio(false);
  std::cout << std::fixed << std::setprecision(0);

  while(true){
    // read number of points
    std::size_t n;
    std::cin >> n;
    if(n==0) break;
    // construct triangulation
    std::vector<K::Point_2> pts(n);
    for (std::size_t i = 0; i < n; ++i) {
      std::cin >> pts[i];
    }
    
    Triangulation t;
    t.insert(pts.begin(),pts.end());

    std::size_t m;
    std::cin >> m;

    for(int i=0;i<m;++i){
      K::Point_2 p;
      std::cin >> p;
      K::FT d = CGAL::squared_distance(p,t.nearest_vertex(p)->point()); 
      std::cout << d << std::endl;
    }

  }
}
