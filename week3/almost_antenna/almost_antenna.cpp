#include <iostream>
#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
#include <CGAL/Min_circle_2.h>
#include <CGAL/Min_circle_2_traits_2.h>
#include <iomanip>
#include <vector>
#include <cmath>
#include <cassert>

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
  std::vector<K::Point_2> new_points;
  std::cout << std::fixed << std::setprecision(0);
  int t = 0;
  while(true){
    std::cin >> n;
    if(n==0) break;

    points.clear();
    new_points.clear();

    for(int i=0;i<n;i++){
      long x,y;
      std::cin >> x >> y;
      points.push_back(K::Point_2(x,y));
    }

    Min_circle mc(points.begin(),points.end(),true);

    // store in new_points all non-supporting points
    for(auto p : points){
      if(std::find(mc.support_points_begin(),mc.support_points_end(),p) ==
                   mc.support_points_end()){
        new_points.push_back(p);
      }
    }

    K::FT best_squared_radius(std::numeric_limits<double>::max());

    // try to leave out one support point at the time
    for(int i=0;i<mc.number_of_support_points();i++){
      Min_circle mc2(new_points.begin(),new_points.end(),true);
      for(int j=0;j<mc.number_of_support_points();j++){
        if(i!=j) mc2.insert(mc.support_point(j));
      }
      Traits::Circle c2 = mc2.circle();
      best_squared_radius = std::min(best_squared_radius,c2.squared_radius());
    }

    std::cout << ceil_to_double(CGAL::sqrt(best_squared_radius)) << std::endl;

  }
}
