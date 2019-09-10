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

int main(){
  while(true){
    int n, m; std::cin >> n >> m;
    if(n==0 && m==0) break;

    Program qp(CGAL::LARGER,true,0,false,0);

    for(int i=0;i<n;++i){
      int c,r; std::cin >> c >> r;
      qp.set_a(i,0,c);
      qp.set_r(0,CGAL::SMALLER);

      qp.set_a(i,1,r);
    }

    for(int i=0;i<n;++i){
      for(int j=0;j<n;++j){
        int v; std::cin >> v;
        if(j<=i)
          qp.set_d(i,j,2*v);
      }
    }

    for(int i=0;i<m;++i){
      int C,R,V;
      std::cin >> C >> R >> V;
      qp.set_b(0,C);
      qp.set_b(1,R);

      Solution s = CGAL::solve_nonnegative_quadratic_program(qp, ET());
      if(s.is_infeasible() || s.objective_value() > V)
        std::cout << "No." << std::endl;
      else
        std::cout << "Yes." << std::endl;
    }
  }
  
}
