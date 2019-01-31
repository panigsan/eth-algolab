#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Min_circle_2.h>
#include <CGAL/Min_circle_2_traits_2.h>
#include <iostream>
#include <vector>
#include <algorithm>

// typedefs
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Min_circle_2_traits_2<K>  Traits;
typedef CGAL::Min_circle_2<Traits>   Min_circle;
typedef K::Point_2 P;
typedef K::FT ET;

// round up to next integer double
double ceil_to_double(const ET& x)
{
  double a = std::ceil(CGAL::to_double(x));
  while (a < x) a += 1;
  while (a-1 >= x) a -= 1;
  return a;
}

void testcase(){
    int n;
    std::cin >> n;

    std::vector<P> houses;
    for(int i=0;i<n;++i){
        long x,y;
        std::cin >> x >> y;
        houses.push_back(P(x,y));
    }

    ET l=0;
    ET r=std::numeric_limits<long>::max();
    ET m;
    ET last_squared;
    while(l!=r){
        m = l+(r-l)/2;
        std::vector<P> to_add;
        for(P house : houses){
            if(CGAL::squared_distance(houses[0],house)>m){
                to_add.push_back(house);
            }
        }
        Min_circle mc(to_add.begin(),to_add.end(),true);
        last_squared = mc.circle().squared_radius();
        if(last_squared == m) break;
        if(last_squared > m){
            if(l==m) break;
            l = m;    
        } else {
            if(r==m) break;
            r = m;
        }
    }
    std::cout <<(long) ceil_to_double(m) << std::endl;

}

int main(){
    std::ios_base::sync_with_stdio(false);
    int T;
    std::cin >> T;
    while(T--) testcase();
}
