// Week 13 - The phantom menace
#include <iostream>
#include <vector>
#include <algorithm>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/edmonds_karp_max_flow.hpp>
// Namespaces
using namespace std;
using namespace boost;


// BGL Graph definitions
// =====================
// Graph Type with nested interior edge properties for Flow Algorithms
typedef	adjacency_list_traits<vecS, vecS, directedS> Traits;
typedef adjacency_list<vecS, vecS, directedS, no_property,
	property<edge_capacity_t, long,
		property<edge_residual_capacity_t, long,
			property<edge_reverse_t, Traits::edge_descriptor> > > >	Graph;
// Interior Property Maps
typedef	property_map<Graph, edge_capacity_t>::type		EdgeCapacityMap;
typedef	property_map<Graph, edge_residual_capacity_t>::type	ResidualCapacityMap;
typedef	property_map<Graph, edge_reverse_t>::type		ReverseEdgeMap;
typedef	graph_traits<Graph>::vertex_descriptor			Vertex;
typedef	graph_traits<Graph>::edge_descriptor			Edge;


// Custom Edge Adder Class, that holds the references
// to the graph, capacity map and reverse edge map
// ===================================================
class EdgeAdder {
	Graph &G;
	EdgeCapacityMap	&capacitymap;
	ReverseEdgeMap	&revedgemap;

public:
	// to initialize the Object
	EdgeAdder(Graph & G, EdgeCapacityMap &capacitymap, ReverseEdgeMap &revedgemap):
		G(G), capacitymap(capacitymap), revedgemap(revedgemap){}

	// to use the Function (add an edge)
	void addEdge(int from, int to, long capacity) {
		Edge e, reverseE;
		bool success;
		tie(e, success) = add_edge(from, to, G);
		tie(reverseE, success) = add_edge(to, from, G);
		capacitymap[e] = capacity;
		capacitymap[reverseE] = 0;
		revedgemap[e] = reverseE;
		revedgemap[reverseE] = e;
	}
};

void testcase() {
	int n, m, s, d, u, v;
	cin >> n >> m >> s >> d;
	Graph G(2 * n);
	Vertex source = add_vertex(G), sink = add_vertex(G);
	EdgeCapacityMap capacitymap = get(edge_capacity, G);
	ReverseEdgeMap revedgemap = get(edge_reverse, G);
	ResidualCapacityMap rescapacitymap = get(edge_residual_capacity, G);
	EdgeAdder eaG(G, capacitymap, revedgemap);
	// High number to allow all possible escape routes, value not really important
	// since max flow needs to be constrained by intra-vertex nodes
	int max_cap = m;
	// Read input and insert corresponding edges
	for(int i = 0; i < m; ++i) {
		cin >> u >> v;
		eaG.addEdge(2 * u + 1, 2 * v, max_cap);
	}
	for(int i = 0; i < s; ++i) {
		cin >> u;
		eaG.addEdge(source, 2 * u, max_cap);
	}
	for(int i = 0; i < d; ++i) {
		cin >> u;
		eaG.addEdge(2 * u + 1, sink, max_cap);
	}
	// Add intra-vertex edge
	for(int i = 0; i < n; ++i) {
		eaG.addEdge(2 * i, 2 * i + 1, 1);
	}

	cout << push_relabel_max_flow(G, source, sink) << endl;
}

int main() {
	int t;
	cin >> t;
	while(t--)
		testcase();
}