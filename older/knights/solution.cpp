// STL includes
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

int m, n;

int in_vertex(int row, int col) {
	return 2 * (col + row * m);
}

int out_vertex(int row, int col) {
	return in_vertex(row, col) + 1;
}

bool is_corner(int row, int col) {
	return (row == 0 && (col == 0 || col == m - 1)) || (row == n - 1 && (col == 0 || col == m - 1));
}

void testcase() {
	int k, c, x, y;
	cin >> m >> n >> k >> c;
	Graph G(2 * m * n);
	EdgeCapacityMap capacitymap = get(edge_capacity, G);
	ReverseEdgeMap revedgemap = get(edge_reverse, G);
	ResidualCapacityMap rescapacitymap = get(edge_residual_capacity, G);
	EdgeAdder eaG(G, capacitymap, revedgemap);
	Vertex source = add_vertex(G), sink = add_vertex(G);

	// Add in-edge for each knight (to in vertex)
	for(int i = 0; i < k; ++i) {
		cin >> x >> y;
		eaG.addEdge(source, in_vertex(y, x), 1);
	}

	// Build internal graph structure
	for(int row = 0; row < n; ++row) {
		for(int col = 0; col < m; ++col) {
			// Add intra-vertex edge
			eaG.addEdge(in_vertex(row, col), out_vertex(row, col), c);
			// Add out-edge
			if(row == 0 || row == n - 1 || col == 0 || col == m - 1)
				eaG.addEdge(out_vertex(row, col), sink, is_corner(row, col)? 2 : 1);
			// Add edge to the left
			if(col > 0)
				eaG.addEdge(out_vertex(row, col), in_vertex(row, col - 1), 1);
			// Add edge to the right
			if(col < m - 1)
				eaG.addEdge(out_vertex(row, col), in_vertex(row, col + 1), 1);
			// Add edge to below
			if(row > 0)
				eaG.addEdge(out_vertex(row, col), in_vertex(row - 1, col), 1);
			// Add edge to above
			if(row < n - 1)
				eaG.addEdge(out_vertex(row, col), in_vertex(row + 1, col), 1);
		}
	}

	// Compute max-flow
	long max_flow = push_relabel_max_flow(G, source, sink);

	// Output
	cout << max_flow << endl;
}

int main() {
	int t;
	cin >> t;
	while(t--)
		testcase();
}