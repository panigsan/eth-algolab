// 2015 - Tracking
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

void testcase() {
	int n, m, k, x, y, a, b, c;
	bool d;
	cin >> n >> m >> k >> x >> y;
	Graph G((k + 1) * n);
	WeightMap weightmap = get(edge_weight, G);	
	for(int i = 0; i < m; ++i) {
		cin >> a >> b >> c >> d;
		// Along river -> go up one layer
		if(d) {
			for(int j = 0; j < k; ++j) {
				weightmap[add_edge(j * n + a, (j + 1) * n + b, G).first] = c;
				weightmap[add_edge(j * n + b, (j + 1) * n + a, G).first] = c;
			}
			// Add also in uppermost layer
			weightmap[add_edge(k * n + a, k * n + b, G).first] = c;
			weightmap[add_edge(k * n + b, k * n + a, G).first] = c;			
		}
		// otherwise -> stay in current layer
		else {
			for(int j = 0; j <= k; ++j) {
				weightmap[add_edge(j * n + a, j * n + b, G).first] = c;
				weightmap[add_edge(j * n + b, j * n + a, G).first] = c;
			}
		}
	}

	// Compute Dijkstra from start in first layer to end in last layer
	vector<int> distmap(num_vertices(G));		// We will use this vector as an Exterior Property Map: Vertex -> Distance to source
	dijkstra_shortest_paths(G, x, distance_map(make_iterator_property_map(distmap.begin(), get(vertex_index, G))));	

	// Output
	cout << distmap[k * n + y] << endl;
}

int main() {
	int t; 
	cin >> t;
	while(t--)
		testcase();
}