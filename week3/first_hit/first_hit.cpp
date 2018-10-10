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
  std::vector<K::Segment_2> segs;

  while(true){
    std::cin >> n;
    if(n==0) break;

    long x,y,a,b;
    std::cin >> x >> y >> a >> b;
    
    const P start(x,y);
    const P somewhere(a,b);
    const K::Ray_2 ray(start,somewhere);

    K::FT best_sq_dist(std::numeric_limits<double>::max());
    P best;
    // read and shuffle the input
    segs.clear();
    for(int i=0;i<n;i++){
      long r,s,t,u;
      std::cin >> r >> s >> t >> u;
      segs.push_back(K::Segment_2(P(r,s),P(t,u)));
    }
    std::random_shuffle(segs.begin(),segs.end());

    for(const K::Segment_2 seg : segs){
      // early stop or skip
      if(best_sq_dist<=K::FT(1)) break;
      if(best_sq_dist< CGAL::squared_distance(start,seg)) continue;

      if(CGAL::do_intersect(ray,seg)){
        auto o = CGAL::intersection(ray,seg);
        if(const P* op = boost::get<P>(&*o)){
          K::FT d(CGAL::squared_distance(start,*op));
          if(d < best_sq_dist){
            best = *op;
            best_sq_dist = d;
          }
        }else if( const S* os = boost::get<S>(&*o)){
          P source = os->source();
          P target = os->target();
          K::FT ds(CGAL::squared_distance(start,source));
          if(ds < best_sq_dist){
            best = source;
            best_sq_dist = ds;
          }
          K::FT dt(CGAL::squared_distance(start,target));
          if(dt < best_sq_dist){
            best = target;
            best_sq_dist = dt;
          }
        } else {
          throw std::runtime_error("should not happen");
        }
      }
    }
    if(best_sq_dist != K::FT(std::numeric_limits<double>::max())) {
      std::cout << floor_to_double(best.x()) << " "
                << floor_to_double(best.y()) << std::endl;
    }
    else      std::cout << "no" << std::endl;
  }
}
