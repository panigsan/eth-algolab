#include <iostream>
#include <iomanip>
#include <cassert>
#include <CGAL/basic.h>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

#define trace(x) std::cerr << #x << " = " << x << std::endl
typedef CGAL::Gmpz ET;
typedef CGAL::Quadratic_program<ET> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;
typedef CGAL::Quotient<ET> SolT;

double ceil_to_double(const SolT& x){
  double a = std::ceil(CGAL::to_double(x));
  while(a<x) ++a;
  while(a-1>=x) --a;
  if(abs(a) == 0) a = 0;
  return a;

}
double floor_to_double(const SolT& x){
  double a = std::floor(CGAL::to_double(x));
  while(a>x) --a;
  while(a+1<=x) ++a;
  return a;
}

void testcase(int p, int a, int b){
  if(p==1){
    Program lp(CGAL::SMALLER,true,0,false,0);

    const int X = 0;
    const int Y = 1;
    lp.set_a(X,0,1); lp.set_a(Y,0,1); lp.set_b(0, 4);
    lp.set_a(X,1,4); lp.set_a(Y,1,2); lp.set_b(1,a*b);
    lp.set_a(X,2,-1); lp.set_a(Y,2,1); lp.set_b(2,1);

    lp.set_c(Y,-b);
    lp.set_d(X,X,2*a); // MAKE SURE TO *2
    Solution s(CGAL::solve_quadratic_program(lp,ET()));

    if(s.is_infeasible())     std::cout << "no" << std::endl;
    else if(s.is_unbounded()) std::cout << "unbounded" << std::endl;
    else std::cout << floor_to_double(-s.objective_value()) << std::endl;

  } else {
    Program lp(CGAL::LARGER,false,0,false,0);

    const int X = 0;
    const int Y = 1;
    const int Z2 = 2;
    lp.set_a(X,0,1); lp.set_a(Y,0,1); lp.set_b(0,-4);
    lp.set_a(X,1,4); lp.set_a(Y,1,2); lp.set_a(Z2,1,1); lp.set_b(1,-a*b);
    lp.set_a(X,2,-1); lp.set_a(Y,2,1); lp.set_b(2,-1);

    lp.set_l(Z2,true,0);
    lp.set_u(X,true,0);
    lp.set_u(Y,true,0);
    lp.set_c(Y,b);
    
    lp.set_d(X,X,2*a);
    lp.set_d(Z2,Z2,2*1);
    Solution s(CGAL::solve_quadratic_program(lp,ET()));

    if(s.is_infeasible())     std::cout << "no" << std::endl;
    else if(s.is_unbounded()) std::cout << "unbounded" << std::endl;
    else std::cout << ceil_to_double(s.objective_value()) << std::endl;
  }

}

int main(){
  std::cout << std::fixed << std::setprecision(0);
  while(true){
    int p,a,b;
    std::cin >> p;
    if(p==0) break;
    std::cin >> a >> b;

    testcase(p,a,b);
  }
}
