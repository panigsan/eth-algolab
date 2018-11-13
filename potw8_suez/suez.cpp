#include <iostream>
#include <cassert>
#include <cmath>
#include <utility>
#include <vector>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

// choose exact integral type
typedef CGAL::Gmpz ET;

// program and solution types
typedef CGAL::Quotient<ET> SolT;
typedef CGAL::Quadratic_program<ET> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

// round up to next integer double
double ceil_to_double(const SolT& x)
{
  double a = std::ceil(CGAL::to_double(x));
  while (a < x) a += 1;
  while (a-1 >= x) a -= 1;
  return a;
}

void testcase(){
  Program lp (CGAL::SMALLER, true, 1, false, 0);

  long n,m,h,w;
  std::cin >> n >> m >> h >> w;

  std::vector<std::pair<long,long>> pos_new(n);
  for(int i=0;i<n;++i){
    long x,y;
    std::cin >> x >> y;
    pos_new[i] = std::make_pair(x,y);
  }
  std::vector<std::pair<long,long>> pos_old(m);
  for(int i=0;i<m;++i){
    long x,y;
    std::cin >> x >> y;
    pos_old[i] = std::make_pair(x,y);
  }

  for(int i=0;i<n;++i){
    lp.set_c(i,-w-h);
  }

  for(int i=0;i<n;++i){
    long xi = pos_new[i].first;
    long yi = pos_new[i].second;
    for(int j=0;j<n;++j){
      if(i==j) continue;
      long xj = pos_new[j].first;
      long yj = pos_new[j].second;
      long dx = std::abs(xi-xj);
      long dy = std::abs(yi-yj);
      if(dx*h < dy*w){
        lp.set_a(j,i*n+j,h);
        lp.set_a(i,i*n+j,h);
        lp.set_b(  i*n+j,2*dy);
      } else {
        lp.set_a(j,i*n+j,w);
        lp.set_a(i,i*n+j,w);
        lp.set_b(  i*n+j,2*dx);
      }
    }

    // look for closest on x and y, considering the width and height
    // of the picture
    long min_x = 1<<30;
    long min_y = 1<<30;

    for(int j=0;j<m;++j){
      long xj = pos_old[j].first;
      long yj = pos_old[j].second;

      long dx = std::abs(xi-xj);
      long dy = std::abs(yi-yj);
      if(dx*h < dy*w) min_y = std::min(min_y,dy);
      else            min_x = std::min(min_x,dx);
    }

    lp.set_a(i,(n*n) + i*2 + 0,h);
    lp.set_b(  (n*n) + i*2 + 0,2*min_y-h);
    lp.set_a(i,(n*n) + i*2 + 1,w);
    lp.set_b(  (n*n) + i*2 + 1,2*min_x-w);

  }

  Solution s = CGAL::solve_linear_program(lp, ET());
  std::cout << ceil_to_double(-2*s.objective_value()) << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cout << std::fixed << std::setprecision(0);

  int T;
  std::cin >> T;
  while(T--) testcase();
  
  return 0;
}
