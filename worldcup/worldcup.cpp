#include <iostream>
#include <cassert>
#include <iomanip>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/intersections.h>
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;

#define trace(x) std::cerr << #x << " = " << x << std::endl

// choose exact integral type
typedef CGAL::Gmpz ET;

// program and solution types
typedef CGAL::Quadratic_program<ET> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;
typedef CGAL::Quotient<ET> SolT;
typedef K::Point_2 P;

double floor_to_double(const SolT& x)
{
  double a = std::floor(CGAL::to_double(x));
	while(a+1<=x) ++a;
	while(a>x) --a;
  return a;
}

void testcase() {

	int n,m,c;
	std::cin >> n >> m >> c;
  Program lp (CGAL::SMALLER, true, 0, false, 0);
	std::vector<P> pos_w;
	std::vector<P> pos_s;
	/*
	std::vector<int> s(n);
	std::vector<int> a(n);
	std::vector<int> u(m);
	std::vector<int> d(m);
	*/
	for(int i=0;i<n;++i){
		int x,y,s,a;
		std::cin >> x >> y >> s >> a;	
    pos_w.push_back(P(x,y));

		lp.set_b(i,s);
		for(int j=0;j<m;++j){
			lp.set_a(i*n+j,i,1);
			lp.set_a(i*n+j,n+j,a);
			lp.set_a(i*n+j,n+m+j,1);
		}
	}
	for(int i=0;i<m;++i){
		int x,y,d,u;
		std::cin >> x >> y >> d >> u;
    pos_s.push_back(P(x,y));

		lp.set_b(n+i,100*u);
		lp.set_b(n+m+i,d);
		lp.set_r(n+m+i,CGAL::EQUAL);
	}

  std::vector<std::vector<double>> rew(n,std::vector<double>(m,0));
	for(int i=0;i<n;++i){
		for(int j=0;j<m;++j){
			double r;
			std::cin >> r;
      rew[i][j] = r;
			//lp.set_c(i*n+j,-r);
		}
	}

  std::vector<std::vector<double>> T(n,std::vector<double>(m,0));
  trace('a');
	for(int i=0;i<c;++i){
		int x,y,r;
		std::cin >> x >> y >> r;
    K::Circle_2 contr(P(x,y),r);

    std::vector<bool> s_ins(m,false);
    for(int k=0;k<m;++k){
      s_ins[k] = CGAL::squared_distance(pos_s[k],contr.center())>r*r;
    }
    for(int j=0;j<n;++j){
      bool w_in = CGAL::squared_distance(pos_w[j],contr.center())>r*r;
      for(int k=0;k<m;++k){
        bool s_in = s_ins[k];
        if(w_in ^ s_in){
          T[j][k] += 1.0;
        }
      }
    }

	}

	for(int i=0;i<n;++i){
		for(int j=0;j<m;++j){
			lp.set_c(i*n+j,-(100.0*rew[i][j]-T[i][j]));
		}
	}

  trace("solving");

  // solve the program, using ET as the exact type
  Solution s = CGAL::solve_nonnegative_linear_program(lp, ET());
	if(s.is_infeasible()){
		std::cout << "RIOT!" << std::endl;	
	}
	else {
		std::cout << floor_to_double(-s.objective_value()/100.0) << std::endl;
	}

  // output solution
}

int main(){
  std::ios_base::sync_with_stdio(false);
	std::cout << std::fixed << std::setprecision(0);
  int T;
  std::cin >> T;
  while(T--) testcase();
}
