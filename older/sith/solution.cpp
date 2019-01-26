#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <cassert>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/prim_minimum_spanning_tree.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/connected_components.hpp>
#include <boost/graph/max_cardinality_matching.hpp>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 P;
typedef CGAL::Triangulation_vertex_base_with_info_2<int,K> Vb;
typedef CGAL::Triangulation_face_base_2<K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>  Triangulation;
typedef Triangulation::Edge_iterator  EdgeIterator;
typedef Triangulation::Vertex_iterator  VertexIterator;
typedef Triangulation::Vertex_handle  VertexHandle;


// Namespaces
using namespace std;
using namespace boost;


// BGL Graph definitions
// =====================
// Graph Type, OutEdgeList Type, VertexList Type, (un)directedS
typedef adjacency_list<vecS, vecS, undirectedS, no_property, no_property>	Graph;
typedef graph_traits<Graph>::edge_descriptor		Edge;		// Edge Descriptor: an object that represents a single edge.
typedef graph_traits<Graph>::vertex_descriptor		Vertex;		// Vertex Descriptor: with vecS vertex list, this is really just an int in the range [0, num_vertices(G)).	
typedef graph_traits<Graph>::edge_iterator		EdgeIt;		// to iterate over all edges
typedef graph_traits<Graph>::out_edge_iterator		OutEdgeIt;	// to iterate over all outgoing edges of a vertex

void testcase() {
	long n, radius, x, y;
	cin >> n >> radius;
	vector<P> planets(n);
	for(int i = 0; i < n; ++i) {
		cin >> x >> y;
		planets[i] = P(x, y);
	}

	// Binary serach for k
	int l = 1, r = n, m = l + (r - l) / 2;
	while(l != r) {
		// Compute delaunay of planets
		Triangulation t;
		t.insert(planets.begin() + m, planets.end());
		int id = 0;
		for (VertexIterator v = t.finite_vertices_begin(); v != t.finite_vertices_end(); ++v, ++id) {
			v -> info() = id;
		}

		// Compute proximity graph
		Graph G(n - m);
		for (EdgeIterator e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e) {
			if(t.segment(*e).squared_length() <= radius * radius) {
				VertexHandle v1 = e->first->vertex((e->second + 1) % 3);
				VertexHandle v2 = e->first->vertex((e->second + 2) % 3);
				add_edge(v1 -> info(), v2 -> info(), G);
			}
		}

		// Compute connected components of proximity graph
		vector<int> componentmap(n - m);
		int ncc = connected_components(G, make_iterator_property_map(componentmap.begin(), get(vertex_index, G))); 
		// Compute components size
		vector<int> componentsize(ncc);
		for (int i = 0; i < componentmap.size(); ++i)
			++componentsize[componentmap[i]];
		// Find size of biggest component
		int max = *max_element(componentsize.begin(), componentsize.end());
		
		// Update intervals
		if(max == m)
			break;
		else if(max > m) {
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

	// Output
	cout << m << endl;
}

int main() {
	int t;
	cin >> t;
	while(t--) {
		testcase();
	}
}