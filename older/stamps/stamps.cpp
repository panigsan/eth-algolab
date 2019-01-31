#include <iostream>
#include <vector>
#include <cassert>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 P;
typedef K::Segment_2 S;

// choose exact rational type
#include <CGAL/Gmpq.h>
typedef CGAL::Gmpq ET;
// solution type the solver provides
typedef CGAL::Quotient<ET> SolT;
// program and solution types
typedef CGAL::Quadratic_program<ET> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;


void testcase(){
    int l,s,w;
    std::cin >> l >> s >> w;

    std::vector<P> lights;
    std::vector<std::pair<P,long>> stamps;
    std::vector<S> walls;
    for(int i=0;i<l;++i){
        long x,y;
        std::cin >> x >> y;
        lights.push_back(P(x,y));
    }
    for(int i=0;i<s;++i){
        long x,y,m;
        std::cin >> x >> y >> m;
        stamps.push_back({P(x,y),m});
    }
    for(int i=0;i<w;++i){
        long x0,y0,x1,y1;
        std::cin >> x0 >> y0 >> x1 >> y1;
        walls.push_back(S(P(x0,y0),P(x1,y1)));
    }
    
    Program lp (CGAL::SMALLER, true, 1, true, 1<<12);

    for(int i=0;i<s;++i){
        std::vector<bool> valid(l,true);
        for(int j=0;j<l;++j){
            for(int k=0;k<w;++k){
                if(CGAL::do_intersect(walls[k],S(stamps[i].first,lights[j]))){
                    valid[j] = false;
                    break;
                }    
            }    
        }
        lp.set_b(2*i,stamps[i].second);
        lp.set_b(2*i+1,1);
        lp.set_r(2*i+1,CGAL::LARGER);
        for(int j=0;j<l;++j){
            if(valid[j]){
                lp.set_a(j,2*i,ET(1) / ET(CGAL::squared_distance(lights[j],stamps[i].first)));    
                lp.set_a(j,2*i+1,ET(1) / ET(CGAL::squared_distance(lights[j],stamps[i].first)));    
            }
        }
    }

    Solution sol = CGAL::solve_linear_program(lp, ET());
    if(sol.is_infeasible()) std::cout << "no" << std::endl;
    else                    std::cout << "yes" << std::endl;
}

int main(){
    std::ios_base::sync_with_stdio(false);
    int T;
    std::cin >> T;
    while(T--) testcase();
}

