#include <iostream>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/squared_distance_2.h>
#include <limits>
#include <iomanip>
#include <cmath>
#include <set>
#include <unordered_set>
#include <utility>
#include <boost/functional/hash.hpp>
typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef K::Point_2 P;
typedef K::Segment_2 S;
typedef std::tuple<long,long,long,long> Key;

struct FourTuple {
  long a,b,c,d;
};

struct MyHash{
  std::size_t operator()(const Key& x) const noexcept {
    using boost::hash_value;
    using boost::hash_combine;
    std::size_t seed = 0;
    hash_combine(seed,hash_value(std::get<0>(x)));
    hash_combine(seed,hash_value(std::get<1>(x)));
    hash_combine(seed,hash_value(std::get<2>(x)));
    hash_combine(seed,hash_value(std::get<3>(x)));

    return seed;
  }
};

double floor_to_double(const K::FT& x){
  double a = std::floor(CGAL::to_double(x));
  while(a>x) --a;
  while(a+1<=x) ++a;
  return a;
}

int main(){
  std::ios_base::sync_with_stdio(false);
  std::cout << std::fixed << std::setprecision(0);
  int n;
  std::unordered_set<std::tuple<long,long,long,long>,MyHash> visited;
  //std::unordered_set<FourTuple,MyHash> visited;
  while(true){
    visited.clear();
    std::cin >> n;
    if(n==0) break;
    long x,y,a,b;
    std::cin >> x >> y >> a >> b;
    
    const P start(x,y);
    const P somewhere(a,b);
    K::Ray_2 ray(start,somewhere);

    bool found = false;
    long mm = std::numeric_limits<long>::max();
    P best;
    for(int i=0;i<n;i++){
      long r,s,t,u;
      std::cin >> r >> s >> t >> u;
      K::Segment_2 seg(P(r,s),P(t,u));
      Key p(r,s,t,u);
      if(visited.find(p) != visited.end()) continue;
      visited.insert(p);
      if(CGAL::do_intersect(ray,seg)){
        auto o = CGAL::intersection(ray,seg);
        if(const P* op = boost::get<P>(&*o)){
          if(!found || CGAL::has_smaller_distance_to_point(start,*op,best)){
            best = *op;
            found = true;
          }
        }else if(const S* os = boost::get<S>(&*o)){
          P source = os->source();
          P target = os->target();
          if(!found || CGAL::has_smaller_distance_to_point(start,source,best)){
            best = source;
            found = true;
          }
          if(!found || CGAL::has_smaller_distance_to_point(start,target,best)){
            best = target;
            found = true;
          }
        } else {
          throw std::runtime_error("should not happen");
        }
      }
    }
the:
    if(found) {
      std::cout << floor_to_double(best.x()) << " "
                << floor_to_double(best.y()) << std::endl;
    }
    else      std::cout << "no" << std::endl;
  }
}
