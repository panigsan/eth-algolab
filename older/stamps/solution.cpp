#include <iostream>
#include <cassert>
#include <CGAL/basic.h>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpzf.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <cmath>

using namespace std;

// choose exact floating-point type
typedef CGAL::Gmpzf ET;

// program and solution types
typedef CGAL::Quadratic_program<double> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;


typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 P;
typedef K::Segment_2 S;

void testcase() {
	int l, s, w;
	cin >> l >> s >> w;
	vector<P> stamps(s), lamps(l);
	vector<S> walls(w);
	vector<int> light(s);
	P p, q;
	for(int i = 0; i < l; ++i) {
		cin >> p;
		lamps[i] = p;
	}
	for(int i = 0; i < s; ++i) {
		cin >> p >> light[i];
		stamps[i] = p;
	}
	for(int i = 0; i < w; ++i) {
		cin >> p >> q;
		walls[i] = S(p, q);
	}

	// Build linear program
	Program lp (CGAL::SMALLER, true, 1, true, pow(2, 12));

	for(int stamp = 0; stamp < s; ++stamp) {
		lp.set_b(2 * stamp, light[stamp]);
		// Total light must be at least 1
		lp.set_b(2 * stamp + 1, 1); lp.set_r(2 * stamp + 1, CGAL::LARGER);
		for(int lamp = 0; lamp < l; ++lamp) {
			bool blocked = false;
			S direct_line = S(lamps[lamp], stamps[stamp]);
			for(int wall = 0; wall < w && !blocked; ++wall) {
				blocked = CGAL::do_intersect(walls[wall], direct_line);
			}
			if(!blocked) {
				double dist = 1.0 / direct_line.squared_length();
				lp.set_a(lamp, 2 * stamp, dist);
				lp.set_a(lamp, 2 * stamp + 1, dist);
			}
		}
	}

	// Solve
	Solution sol = CGAL::solve_linear_program(lp, ET());
  	assert (sol.solves_linear_program(lp));
	
	// Output
	if(sol.is_optimal())
		cout << "yes" << endl;
	else
		cout << "no" << endl;

}


int main() {
	int t;
	cin >> t;
	while(t--)
		testcase();
}