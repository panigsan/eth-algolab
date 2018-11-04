#include <iostream>
#include <iomanip>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>

// choose exact rational type
#include <CGAL/Gmpq.h>
typedef CGAL::Gmpq ET;
// solution type the solver provides
typedef CGAL::Quotient<ET> SolT;
// program and solution types
typedef CGAL::Quadratic_program<ET> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

#define trace(x) std::cerr << #x << " = " << x << std::endl

double floor_to_double(const SolT& x){
  double a = std::floor(CGAL::to_double(x));
  while(a>x) --a;
  while(a+1<=x) ++a;
  return a;
}

int main(){
  std::ios_base::sync_with_stdio(false);
  std::cout << std::fixed << std::setprecision(0);
  while(true){
    int n, m;
    std::cin >> n >> m;
    if(n==0 && m==0) break;

    Program lp(CGAL::SMALLER, true, 0, false, 0);

    for(int j=0;j<n;++j) {
      int nut_min, nut_max;
      std::cin >> nut_min >> nut_max;
      lp.set_b(j,nut_max);

      lp.set_b(n+j,nut_min);
      lp.set_r(n+j,CGAL::LARGER);
    }
    
    for(int i=0;i<m;++i){
      int p; std::cin >> p;

      lp.set_c(i,p);

      for(int j=0;j<n;++j){
        int nut; std::cin >> nut;
        lp.set_a(i,j,nut);
        lp.set_a(i,n+j,nut);
      }
    }
    Solution s = CGAL::solve_nonnegative_quadratic_program(lp, ET());
    if(s.is_infeasible()) std::cout << "No such diet." << std::endl;
    else std::cout << floor_to_double(s.objective_value()) << std::endl;
  }

}
