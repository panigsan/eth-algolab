#include <iostream>
#include <iomanip>
#include <vector>
#include <cassert>
#include <cmath>
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

#define trace(x) std::cerr << #x << " = "  << x << std::endl

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
    int n,d;
    std::cin >> n;
    if(n==0) break;
    std::cin >> d;

    std::vector<std::vector<int>> A(n,std::vector<int>(d+1));
    for(int i=0;i<n;++i){
      for(int j=0;j<d+1;++j){
        std::cin >> A[i][j];
      }
    }

    Program qp(CGAL::SMALLER,false,0,false,0);
    /*
    for(int i=0;i<n;++i){
      for(int j=0;j<d;++j){
        qp.set_a(j,i,A[i][j]);
      }
      qp.set_b(i,A[i][d]);
    }
    */

    for(int i=0;i<n;++i){
      int a2=0;
      for(int j=0;j<d;++j){
        qp.set_a(j,n+i,A[i][j]);
        a2 += A[i][j]*A[i][j];
      }
      qp.set_a(d,n+i,std::sqrt(a2));
      qp.set_b(n+i,A[i][d]);
    }

    qp.set_l(d,true,0);
    qp.set_c(d,-1);

    Solution s = CGAL::solve_linear_program(qp, ET());

    if(s.is_unbounded()) std::cout << "inf" << std::endl;
    else if(s.is_infeasible()) std::cout << "none" << std::endl;
    else{
      std::cout << floor_to_double(-s.objective_value()) << std::endl;
    }

  }

}
