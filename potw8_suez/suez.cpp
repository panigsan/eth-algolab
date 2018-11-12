#include <iostream>
#include <cassert>
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

  int n,m,h,w;
  std::cin >> n >> m >> h >> w;

  std::vector<std::pair<int,int>> pos_new(n);
  for(int i=0;i<n;++i){
    int x,y;
    std::cin >> x >> y;
    pos_new[i] = std::make_pair(x,y);
  }
  std::vector<std::pair<int,int>> pos_old(n);
  for(int i=0;i<m;++i){
    int x,y;
    std::cin >> x >> y;
    pos_old[i] = std::make_pair(x,y);
  }

  for(int i=0;i<n;++i){
    lp.set_l(i,true,1);
    lp.set_c(i,-w-h);
  }

  for(int i=0;i<n;++i){
    int xi = pos_new[i].first;
    int yi = pos_new[i].second;
    for(int j=0;j<n;++j){
      if(i==j) continue;
      int xj = pos_new[j].first;
      int yj = pos_new[j].second;
      if(xj < xi){
        lp.set_a(j,i*n+j,w);
        lp.set_a(i,i*n+j,w);
        lp.set_b(i*n+j,2*(xi - xj));
      } else{
        lp.set_a(i,i*n+j,w);  
        lp.set_a(j,i*n+j,w);  
        lp.set_b(  i*n+j,2*(xj - xi));
      }
      if(yj < yi){
        lp.set_a(j,(n*n) + i*n + j,h); 
        lp.set_a(i,(n*n) + i*n + j,h); 
        lp.set_b(  (n*n) + i*n + j,2*(yi-yj));
      } else {
        lp.set_a(j,(n*n) + i*n + j,h); 
        lp.set_a(i,(n*n) + i*n + j,h); 
        lp.set_b(  (n*n) + i*n + j,2*(yj-yi));
      }
    }
  }

  Solution s = CGAL::solve_linear_program(lp, ET());
  Solution::Variable_value_iterator opt = s.variable_values_begin();
  trace(ceil_to_double(s.objective_value()));
  for(int i=0;i<n;++i) trace(ceil_to_double(*(opt+i)));
  std::cerr << s << std::endl;
  std::cout << ceil_to_double(-2*s.objective_value()) << std::endl;


}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cout << std::fixed << std::setprecision(0);

  int T;
  std::cin >> T;
  while(T--) testcase();
  
  // by default, we have a nonnegative LP with Ax <= b
  /*
  Program lp (CGAL::SMALLER, true, 0, false, 0);

  // now set the non-default entries
  const int X = 0;
  const int Y = 1;
  lp.set_a(X, 0,  1); lp.set_a(Y, 0, 1); lp.set_b(0, 7);  //  x + y  <= 7
  lp.set_a(X, 1, -1); lp.set_a(Y, 1, 2); lp.set_b(1, 4);  // -x + 2y <= 4
  lp.set_u(Y, true, 4);                                   //       y <= 4
  lp.set_c(Y, -32);                                       // -32y
  lp.set_c0(64);                                          // +64

  // solve the program, using ET as the exact type
  Solution s = CGAL::solve_linear_program(lp, ET());
  assert (s.solves_linear_program(lp));

  // output solution
  std::cout << s;
  */
  return 0;
}
