#include <iostream>
#include <cassert>
#include <CGAL/basic.h>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>
#include <limits>

using namespace std;

// choose exact floating-point type
typedef CGAL::Gmpz ET;

// program and solution types
typedef CGAL::Quadratic_program<long> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

void testcase(int n, int m) {
	int c, r, v;
	Program qp (CGAL::LARGER, true, 0, false, 0);
	qp.set_r(1, CGAL::SMALLER);
	// Build common program
	for(int i = 0; i < n; ++i) {
		cin >> c >> r;
		qp.set_a(i, 0, r);
		qp.set_a(i, 1, c);
	}
	for(int i = 0; i < n; ++i)
		for(int j = 0; j < n; ++j){
			cin >> v;
			if(j <= i)
				qp.set_d(i, j, 2 * v);
		}

	// For each friend adapt and solve program
	for(int i = 0; i < m; ++i) {
		cin >> c >> v;
		qp.set_b(1, c);
		// Binary search for max R such that associated risk <= v
		int l = numeric_limits<int>::min(), r = numeric_limits<int>::max(), m = (l + r) / 2;
		while(l != r) {
			qp.set_b(0, m);

			Solution s = CGAL::solve_quadratic_program(qp, ET());
  			assert (s.solves_quadratic_program(qp));

  			if(s.is_optimal() && s.objective_value() == v)
  				break;
  			else if(s.is_optimal() && s.objective_value() < v) {
  				if(l == m)
  					break;
  				l = m;
  			}
  			else 
  				r = m;
  			m = (l + r) / 2;
		}

		cout << m << endl;
	}
}

int main() {
	int n, m;
	cin >> n >> m;
	while(n && m) {
		testcase(n, m);
		cin >> n >> m;
	}
}