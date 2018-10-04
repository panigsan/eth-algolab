#include <iostream>
#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
#include <CGAL/Min_circle_2.h>
#include <CGAL/Min_circle_2_traits_2.h>
#include <iomanip>
#include <vector>
#include <cmath>

typedef CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt K;
typedef CGAL::Min_circle_2_traits_2<K> Traits;
typedef CGAL::Min_circle_2<Traits> Min_circle;

double ceil_to_double(const K::FT& x){
  double a = std::ceil(CGAL::to_double(x));
  while(a-1>x) --a;
  while(a<x) ++a;
  return a;
}

int main(){
  int n;
  std::vector<K::Point_2> points;
  std::cout << std::fixed << std::setprecision(0);
  while(true){
    std::cin >> n;
    if(n==0) break;
    points.clear();
    for(int i=0;i<n;i++){
      long x,y;
      std::cin >> x >> y;
      points.push_back(K::Point_2(x,y));
    }

    Min_circle mc(points.begin(),points.end(),true);
    Traits::Circle c = mc.circle();

    std::cout << ceil_to_double(CGAL::sqrt(c.squared_radius())) << std::endl;

  }
}
