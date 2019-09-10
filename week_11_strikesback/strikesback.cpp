#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

#define trace(x) std::cerr << #x << " = " << x << std::endl
// USE GMPQ otherwise it doens't work!!!
typedef CGAL::Gmpq ET;

// program and solution types
typedef CGAL::Quadratic_program<ET> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;

void testcase()
{
  // read number of points
  int a,s,b;
  long e;
  std::cin >> a >> s >> b;
  std::cin >> e;
  // read points
  std::vector<std::pair<K::Point_2,long>> parts;
  for(int i=0;i<a;++i){
    long x,y,d;
    std::cin >> x >> y >> d;
    parts.push_back(std::make_pair(K::Point_2(x,y),d));
  }
  std::vector<K::Point_2> shoots;
  for(int i=0;i<s;++i){
    long x,y;
    std::cin >> x >> y;
    shoots.push_back(K::Point_2(x,y));
  }
  std::vector<K::Point_2> hunts;
  for(int i=0;i<b;++i){
    long x,y;
    std::cin >> x >> y;
    hunts.push_back(K::Point_2(x,y));
  }

  Triangulation t;
  t.insert(hunts.begin(), hunts.end());
  std::vector<K::FT> max_r2;
  for(int i=0;i<s;++i){
    if(b==0) {
      max_r2.push_back(std::numeric_limits<long>::max());
    }else {
      K::Point_2 hunter = t.nearest_vertex(shoots[i])->point();
      max_r2.push_back(CGAL::squared_distance(shoots[i],hunter));
    }
  }

  Program lp (CGAL::LARGER, true, 0, false, 0);
  for(int i=0;i<s;++i){
    lp.set_c(i,1);
    for(int j=0;j<a;++j){
      K::FT dist2 = CGAL::squared_distance(shoots[i],parts[j].first);
      if(dist2<max_r2[i]){
        if(dist2==0) dist2 = 1;
        lp.set_a(i,j,ET(1) / ET(dist2));
      }
    }
  }
  for(int j=0;j<a;++j){
    lp.set_b(j,parts[j].second);
  }


  Solution sol = CGAL::solve_linear_program(lp, ET());
  if(sol.is_infeasible() || sol.objective_value() > ET(e)) std::cout << 'n' << std::endl;
  else std::cout << 'y' << std::endl;
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int T;
  std::cin >> T;
  while(T--) testcase();
}
