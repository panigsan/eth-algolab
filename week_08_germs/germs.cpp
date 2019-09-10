#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

#define trace(x) std::cerr << #x << " = " << x << std::endl
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;
typedef Triangulation::Vertex_iterator  Vertex_iterator;


int main()
{
  std::cout << std::fixed << std::setprecision(0);
  while(true){

    // read number of points
    std::size_t n;
    std::cin >> n;
    if(n==0) break;

    long long L,B,R,T;
    std::cin >> L >> B >> R >> T;
    // read points
    std::vector<K::Point_2> pts(n);
    for (std::size_t i = 0; i < n; ++i) {
      std::cin >> pts[i];
    }
    // construct triangulation
    Triangulation t;
    t.insert(pts.begin(), pts.end());

    std::vector<double> V;
    V.reserve(n);
    for(auto f = t.finite_vertices_begin(); f!=t.finite_vertices_end(); ++f){
      double closest = std::numeric_limits<double>::max();
      Triangulation::Edge_circulator c = t.incident_edges(f);
      if(c!=0){ // if it has at least one incident edge
        do{
          if(!t.is_infinite(c)) {
            closest = std::min(closest,t.segment(c).squared_length()); 
          }
        } while (++c != t.incident_edges(f));
      }

      closest = closest/4;
      closest = std::min(closest, std::pow(std::abs(f->point().x() - L),2));
      closest = std::min(closest, std::pow(std::abs(f->point().x() - R),2));
      closest = std::min(closest, std::pow(std::abs(f->point().y() - B),2));
      closest = std::min(closest, std::pow(std::abs(f->point().y() - T),2));

      V.push_back(closest);

    }

    std::sort(V.begin(),V.end());

    std::cout << std::ceil(std::sqrt(std::sqrt(V[0+0])-0.5)) << " ";
    std::cout << std::ceil(std::sqrt(std::sqrt(V[n/2])-0.5)) << " ";
    std::cout << std::ceil(std::sqrt(std::sqrt(V[n-1])-0.5)) << std::endl;


  }
}
