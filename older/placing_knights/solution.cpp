#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>

// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/tuple/tuple.hpp>
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
typedef	graph_traits<Graph>::out_edge_iterator			OutEdgeIt;

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
	int n;
	cin >> n;
	// Get input
	vector<vector<bool> > present(n, vector<bool>(n));
	bool b;
	int holes = 0;
	for(int i = 0; i < n; ++i) {
		for(int j = 0; j < n; ++j) {
			//cin >> present[i][j];
			cin >> b;
			present[i][j] = b;
			if(!b)
				holes++;
		}
	}

	// Build threaten graph
	Graph G(n * n);
	EdgeCapacityMap capacitymap = get(edge_capacity, G);
	ReverseEdgeMap revedgemap = get(edge_reverse, G);
	ResidualCapacityMap rescapacitymap = get(edge_residual_capacity, G);
	EdgeAdder eaG(G, capacitymap, revedgemap);
	Vertex source = add_vertex(G), sink = add_vertex(G);
	for(int i = 0; i < n; ++i) {
		for(int j = 0; j < n; ++j) {
			// add edges only from one partition to the other
			int curr = j + n * i;
			if((i + j) % 2 == 0 && present[i][j]) {
				eaG.addEdge(source, curr, 1);
				// [i−1,j−2]
				if(i - 1 >= 0 && j - 2 >= 0 && present[i - 1][j - 2])
					eaG.addEdge(curr, j - 2 + n * (i - 1), 1);
				// [i−1,j+2]
				if(i - 1 >= 0 && j + 2 < n && present[i - 1][j + 2])
					eaG.addEdge(curr, j + 2 + n * (i - 1), 1);
				// [i+1,j−2]
				if(i + 1 < n && j - 2 >= 0 && present[i + 1][j - 2])
					eaG.addEdge(curr, j - 2 + n * (i + 1), 1);
				// [i+1,j+2]
				if(i + 1 < n && j + 2 < n && present[i + 1][j + 2])
					eaG.addEdge(curr, j + 2 + n * (i + 1), 1);
				// [i−2,j−1]
				if(i - 2 >= 0 && j - 1 >= 0 && present[i - 2][j - 1])
					eaG.addEdge(curr, j - 1 + n * (i - 2), 1);
				// [i−2,j+1]
				if(i - 2 >= 0 && j + 1 < n && present[i - 2][j + 1])
					eaG.addEdge(curr, j + 1 + n * (i - 2), 1);
				// [i+2,j−1]
				if(i + 2 < n && j - 1 >= 0 && present[i + 2][j - 1])
					eaG.addEdge(curr, j - 1 + n * (i + 2), 1);
				// [i+2,j+1]
				if(i + 2 < n && j + 1 < n && present[i + 2][j + 1])
					eaG.addEdge(curr, j + 1 + n * (i + 2), 1);
			}
			else {
				eaG.addEdge(curr, sink, 1);
			}
		}
	}


	// Compute size of minimum vertex cover
	push_relabel_max_flow(G, source, sink);

	// BFS to find vertex set S
	vector<int> vis(n * n, false); // visited flags
	std::queue<int> Q; // BFS queue (from std:: not boost::)
	vis[source] = true; // Mark the source as visited
	Q.push(source);
	while (!Q.empty()) {
		const int u = Q.front();
		Q.pop();
		OutEdgeIt ebeg, eend;
		for (tie(ebeg, eend) = out_edges(u, G); ebeg != eend; ++ebeg) {
			const int v = target(*ebeg, G);
			// Only follow edges with spare capacity
			if (rescapacitymap[*ebeg] == 0 || vis[v]) continue;
			vis[v] = true;
			Q.push(v);
		}
	}

	int mvc = 0;
	for(int i = 0; i < n; ++i) {
		for(int j = 0; j < n; ++j) {
			// Count position if present and in left partition but not visited or in right partition and visited
			if(present[i][j] && (((i + j) % 2 == 0 && !vis[j + n * i]) || ((i + j) % 2 == 1 && vis[j + n * i])))
				mvc++;
		}
	}

	// Compute size of maximum independent set using Koenig's theorem
	int mis = n * n - holes - mvc;

	// Output
	cout << mis << endl;
}

int main() {
	int t;
	cin >> t;
	while(t--)
		testcase();
}
