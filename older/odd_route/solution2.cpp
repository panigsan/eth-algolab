// Week 10 - Odd Route
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
// Namespaces
using namespace std;
using namespace boost;


// BGL Graph definitions
// =====================
// Graph Type, OutEdgeList Type, VertexList Type, (un)directedS
typedef adjacency_list<vecS, vecS, directedS,		// Use vecS for the VertexList! Choosing setS for the OutEdgeList disallows parallel edges.
		no_property,				// interior properties of vertices	
		property<edge_weight_t, int> 		// interior properties of edges
		>					Graph;
typedef graph_traits<Graph>::edge_descriptor		Edge;		// Edge Descriptor: an object that represents a single edge.
typedef graph_traits<Graph>::vertex_descriptor		Vertex;		// Vertex Descriptor: with vecS vertex list, this is really just an int in the range [0, num_vertices(G)).	
typedef graph_traits<Graph>::edge_iterator		EdgeIt;		// to iterate over all edges
typedef graph_traits<Graph>::out_edge_iterator		OutEdgeIt;	// to iterate over all outgoing edges of a vertex
typedef property_map<Graph, edge_weight_t>::type	WeightMap;	// property map to access the interior property edge_weight_t

void addEdge(int u, int v, int w, Graph& G, WeightMap& wm) {
	Edge e = add_edge(u, v, G).first;
	wm[e] = w;
}

void testcase() {
	int n, m, s, t, x, y, w;
	cin >> n >> m >> s >> t;
	Graph G(4 * n);
	WeightMap weightmap = get(edge_weight, G);
	int ee = 0, oe = n, eo = 2 * n, oo = 3 * n;
	for(int i = 0; i < m; ++i) {
		cin >> x >> y >> w;
		if(w % 2 == 0) {
			addEdge(ee + x, oe + y, w, G, weightmap);
			addEdge(eo + x, oo + y, w, G, weightmap);
			addEdge(oe + x, ee + y, w, G, weightmap);
			addEdge(oo + x, eo + y, w, G, weightmap);
		}
		else {
			addEdge(ee + x, oo + y, w, G, weightmap);
			addEdge(eo + x, oe + y, w, G, weightmap);
			addEdge(oe + x, eo + y, w, G, weightmap);
			addEdge(oo + x, ee + y, w, G, weightmap);
		}
	}

	// Call Dijkstra from s in ee
	vector<int> distmap(4 * n);	
	dijkstra_shortest_paths(G, ee + s, distance_map(make_iterator_property_map(distmap.begin(), get(vertex_index, G))));

	// TODO: Output
	if(distmap[oo + t] < INT_MAX)
		cout << distmap[oo + t] << endl;
	else
		cout << "no" << endl;
}

int main() {
	int t;
	cin >> t;
	while(t--)
		testcase();
}