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

#define trace(x) std::cerr << #x << " = " << x << std::endl

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
  std::vector<std::pair<int,int>> pos_old(n);
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
      if(std::abs(xi-xj)*h < std::abs(yi-yj)*w){
        lp.set_a(j,i*n+j,h);
        lp.set_a(i,i*n+j,h);
        lp.set_b(  i*n+j,2*std::abs(yi-yj));
      } else {
        lp.set_a(j,i*n+j,w);
        lp.set_a(i,i*n+j,w);
        lp.set_b(  i*n+j,2*std::abs(xj-xi));
      }
    }

    for(int j=0;j<m;++j){
      long xj = pos_old[j].first;
      long yj = pos_old[j].second;
      if(std::abs(xi-xj)*h < std::abs(yi-yj)*w){
        lp.set_a(j,(n*n) + i*n+j,h);
        lp.set_a(i,(n*n) + i*n+j,h);
        lp.set_b(  (n*n) + i*n+j,2*std::abs(yi-yj));
      } else {
        lp.set_a(j,(n*n) + i*n+j,w);
        lp.set_a(i,(n*n) + i*n+j,w);
        lp.set_b(  (n*n) + i*n+j,2*std::abs(xj-xi));
      }
    }
  }

  Solution s = CGAL::solve_linear_program(lp, ET());
  Solution::Variable_value_iterator opt = s.variable_values_begin();
  //trace(ceil_to_double(s.objective_value()));
  //for(int i=0;i<n;++i) trace(ceil_to_double(*(opt+i)));
  if(s.objective_value()>0)
    std::cerr << s << std::endl;
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
