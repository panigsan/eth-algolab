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

    /*
    for(auto p : points){
      if(std::find(mc.support_points_begin(),mc.support_points_end(),p) ==
                   mc.support_points_end()){
        new_points.push_back(p);
      }
    }

    Min_circle new_mc(new_points.begin(),new_points.end(),true);
    */
    K::FT best_squared_radius(std::numeric_limits<double>::max());
    for(int i=0;i<mc.number_of_support_points();i++){
      new_points.clear();
      for(auto p : points){
        if(std::find(mc.support_points_begin(),mc.support_points_end(),p) ==
                     mc.support_points_end()){
          new_points.push_back(p);
        }
      }
      //Min_circle mc2(new_mc.support_points_begin(),new_mc.support_points_end());
      Min_circle mc2(new_points.begin(),new_points.end(),true);
      for(int j=0;j<mc.number_of_support_points();j++){
        if(i!=j) mc2.insert(mc.support_point(j));
      }
      Traits::Circle c2 = mc2.circle();
      auto rad = c2.squared_radius();
      //std::cout << ceil_to_double(CGAL::sqrt(rad)) << std::endl;
      if(rad < best_squared_radius){
        best_squared_radius = rad;
      }
    }
    if(false && t==3){
      int cnt = 0;
      for(auto p : points){
        if(mc.has_on_boundary(p)) ++cnt;
      }
      //std::cout << cnt << std::endl;
      //std::cout << mc.number_of_support_points() << std::endl;
    }

    std::cout << ceil_to_double(CGAL::sqrt(best_squared_radius)) << std::endl;

    t++;
  }
}
