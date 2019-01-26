// Week 4 - TheeV
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Min_circle_2.h>
#include <CGAL/Min_circle_2_traits_2.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>

using namespace std;

// typedefs
typedef  CGAL::Exact_predicates_exact_constructions_kernel K;
typedef  CGAL::Min_circle_2_traits_2<K>  Traits;
typedef  CGAL::Min_circle_2<Traits>      Min_circle;
typedef  K::Point_2 P;

double ceil_to_double(const K::FT& x)
{
  double a = ceil(CGAL::to_double(x));
  while (a < x) a += 1;
  while (a-1 > x) a -= 1;
  return a;
}

void testcase() {
	int n;
	cin >> n;

	vector<P> cities(n);
	int x, y;
	for (int i = 0; i < n; ++i) {
		cin >> x >> y;
		cities[i] = P(x, y);
	}

	K::FT min_r = 0;
	if(n > 2) {
		// Compute (squared) distance from first transmitter to each city
		vector<pair<K::FT,P> > dist2p(n);
		for(int i = 0; i < n; ++i) {
			K::FT d = CGAL::squared_distance(cities[0], cities[i]);
			dist2p[i] = make_pair(d, cities[i]);
		}

		sort(dist2p.begin(), dist2p.end());

		// Sorted vectors for distances and corresponding point
		vector<K::FT> dist2(n);
		vector<P> ps(n);
		for(int i = 0; i < n; ++i) {
			dist2[i] = dist2p[i].first;
			ps[i] = dist2p[i].second;
		}

		// Binary search for the radius
		min_r = dist2.back();
		vector<P>::iterator first = ps.begin();
		int l = 0, r = dist2.size() - 1, m = l + (r - l) / 2;
		while(l != r){
			Min_circle mc(first + m + 1, ps.end(), true);
			Traits::Circle c = mc.circle();
			min_r = min(min_r, max(c.squared_radius(), dist2[m]));
			if(c.squared_radius() == dist2[m]) {
				break;
			}
			else if(c.squared_radius() > dist2[m]) {
				if(l == m)
					break;
				l = m;
			}
			else {
				if(r == m)
					break;
				r = m;
			}
			m = l + (r - l) / 2;
		}
	}

	// Output
	cout << (long) ceil_to_double(min_r) << endl;
}

int main() {
	ios_base::sync_with_stdio(false);
	int t;
	cin >> t;
	while(t--)
		testcase();
}