#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <cmath>
#include <iomanip>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;

double ceil_to_double(const K::FT& x){
  double a = std::ceil(CGAL::to_double(x));
  while(a<x)++a;
  while(a-1>=x) --a;
  return a;
}

int main()
{
  std::ios_base::sync_with_stdio(false);
  std::cout << std::fixed << std::setprecision(0);
  while(true){
    // read number of points
    std::size_t n;
    std::cin >> n;
    if(n==0) break;

    // read points
    std::vector<K::Point_2> pts;
    pts.reserve(n);
    for (std::size_t i = 0; i < n; ++i) {
      K::Point_2 p;
      std::cin >> p;
      pts.push_back(p);
    }
    // construct triangulation
    Triangulation t;
    t.insert(pts.begin(), pts.end());
    // output all edges
    K::FT best(std::numeric_limits<long>::max());
    for (Edge_iterator e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e){
      K::FT sqrd = t.segment(e).squared_length();
      if(sqrd < best) best = sqrd;
      //std::cout << t.segment(e) << "\n";
    }
    std::cout << ceil_to_double(CGAL::sqrt(best) * 50) << std::endl;
  }
}
