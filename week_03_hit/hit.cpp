#include <iostream>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;

int main(){
  int n;
  while(true){
    std::cin >> n;
    if(n==0) break;
    long x,y,a,b;
    std::cin >> x >> y >> a >> b;
    
    K::Point_2 start(x,y);
    K::Point_2 somewhere(a,b);
    K::Ray_2 ray(start,somewhere);

    bool do_int = false;
    for(int i=0;i<n;i++){
      long r,s,t,u;
      std::cin >> r >> s >> t >> u;
      K::Segment_2 seg(K::Point_2(r,s),K::Point_2(t,u));
      if(!do_int && CGAL::do_intersect(ray,seg)){
        do_int = true;
      }
    }
    std::cout << (do_int ? "yes" : "no") << std::endl;
  }
}
