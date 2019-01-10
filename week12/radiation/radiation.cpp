// example: decide whether there exists a disk that covers a given set
// of points R in the plane and is disjoint from another set of points B
#include <iostream>
#include <vector>
#include <cassert>
#include <cmath>
#include <utility>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>

#include <CGAL/Gmpq.h>
typedef CGAL::Gmpz ET;

// program and solution types
typedef CGAL::Quadratic_program<ET> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

#define trace(x) std::cerr << #x << " = " << x << std::endl

#define V std::vector

struct P {
  ET x,y,z;
};

P pow(V<V<std::pair<bool,P>>>& pows,int idx, int power){
  if(power==0) return pows[idx][0].second;
  if(pows[idx][power].first) return pows[idx][power].second;
  P prev = pow(pows,idx,power-1);
  prev.x *= pows[idx][1].second.x;
  prev.y *= pows[idx][1].second.y;
  prev.z *= pows[idx][1].second.z;
  pows[idx][power].second = prev;
  pows[idx][power].first = true;
  return prev;
}


void testcase() {
  int h,t;
  std::cin >> h >> t;
  std::vector<P> H;
  std::vector<P> T;

  std::pair<bool,P> init = std::make_pair(false,P({1,1,1}));
  V<V<std::pair<bool,P>>> powH(h,V<std::pair<bool,P>>(30,init));
  V<V<std::pair<bool,P>>> powT(t,V<std::pair<bool,P>>(30,init));
  for(int i=0;i<h;++i){
    ET x,y,z;
    std::cin >> x >> y >> z;
    powH[i][1].second = {x,y,z};
    powH[i][1].first = true;
    H.push_back({x,y,z});
  }
  for(int i=0;i<t;++i){
    ET x,y,z;
    std::cin >> x >> y >> z;
    powT[i][1].second = {x,y,z};
    powT[i][1].first = true;
    T.push_back({x,y,z});
  }

  for(int d=1;d<30;++d){
    Program lp (CGAL::SMALLER,false,0,false,0);
    long count;
    for(int p=0;p<h;++p){
      count=0;
      lp.set_b(p,-1);
      for(int i=0;i<d;++i){
        for(int j=0;j<d;++j){
          for(int k=0;k<d;++k){
            if(i+j+k<d){
              lp.set_a(count++,p,-pow(powH,p,i).x*pow(powH,p,j).y*pow(powH,p,k).z);
            }
          }
        }
      }
    }
    for(int p=0;p<t;++p){
      count=0;
      lp.set_b(h+p,1);
      lp.set_r(h+p,CGAL::LARGER);
      for(int i=0;i<d;++i){
        for(int j=0;j<d;++j){
          for(int k=0;k<d;++k){
            if(i+j+k<d){
              lp.set_a(count++,h+p,-pow(powT,p,i).x*pow(powT,p,j).y*pow(powT,p,k).z);
            }
          }
        }
      }
    }

	  CGAL::Quadratic_program_options options;
	  options.set_pricing_strategy(CGAL::QP_BLAND);
    Solution s = CGAL::solve_linear_program(lp,ET(),options);
    if(!s.is_infeasible()){
      std::cout << (d-1) << std::endl;
      return;
    }

  }
  std::cout << "Impossible!" << std::endl;
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int T;
  std::cin >> T;
  while(T--) testcase();
}
