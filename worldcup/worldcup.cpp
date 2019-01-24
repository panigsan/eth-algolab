#include <iostream>
#include <cassert>
#include <iomanip>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpq.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

#define trace(x) std::cerr << #x << " = " << x << std::endl

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;

// choose exact integral type
typedef CGAL::Gmpq ET;

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
  // Read warehouses
  for(int i=0;i<n;++i){
    long x,y,s,a;
    std::cin >> x >> y >> s >> a; 
    
    pos_w.push_back(P(x,y));

    lp.set_b(i,s);
    for(int j=0;j<m;++j){
      lp.set_a(i*m+j,i,1);     // limit supply
      lp.set_a(i*m+j,n+j,a);   // limit alchool
      lp.set_a(i*m+j,n+m+j,1); // equal liters
    }
  }
  // Read stadions
  for(int i=0;i<m;++i){
    long x,y,d,u;
    std::cin >> x >> y >> d >> u;
    pos_s.push_back(P(x,y));

    lp.set_b(n+i,100.0*u);       // limit alchool
    lp.set_b(n+m+i,d);           // equal lister 
    lp.set_r(n+m+i,CGAL::EQUAL);
  }

  std::vector<std::vector<double>> rew(n,std::vector<double>(m,0));
  // Read reward
  for(int i=0;i<n;++i){
    for(int j=0;j<m;++j){
      double r;
      std::cin >> r;
      rew[i][j] = r;
    }
  }

  Triangulation t;
  t.insert(pos_w.begin(),pos_w.end());
  t.insert(pos_s.begin(),pos_s.end());

  for(int i=0;i<c;++i){
    long x,y,r;
    std::cin >> x >> y >> r;
    P center(x,y);
    K::Circle_2 contr(P(x,y),r);

    auto vh = t.nearest_vertex(center);
    if(CGAL::squared_distance(vh->point(),center)>=r*r) continue;

    for(int j=0;j<n;++j){
      bool w_in = CGAL::squared_distance(pos_w[j],center)<=r*r;
      for(int k=0;k<m;++k){
        bool s_in = CGAL::squared_distance(pos_s[k],center)<=r*r;
        if(w_in != s_in){
          rew[j][k] -= 0.01;
        }
      }
    }

  }

  for(int i=0;i<n;++i){
    for(int j=0;j<m;++j){
      lp.set_c(i*m+j,-rew[i][j]);
    }
  }

  // solve the program, using ET as the exact type
  Solution s = CGAL::solve_nonnegative_linear_program(lp, ET());
  if(s.is_infeasible()){
    std::cout << "RIOT!" << std::endl;  
  }
  else {
    std::cout << floor_to_double(-s.objective_value()) << std::endl;
  }

}

int main(){
  std::ios_base::sync_with_stdio(false);
  std::cout << std::fixed << std::setprecision(0);
  int T;
  std::cin >> T;
  while(T--) testcase();
}

