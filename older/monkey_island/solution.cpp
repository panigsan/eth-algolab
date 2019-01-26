// 2015 - Monkey Island
#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <stack>

// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/strong_components.hpp>

// Namespaces
using namespace std;
using namespace boost;


// Directed graph with integer weights on edges.
typedef adjacency_list<vecS, vecS, directedS,no_property,no_property>	Graph;
typedef graph_traits<Graph>::vertex_descriptor		Vertex;	// Vertex type		
typedef graph_traits<Graph>::edge_descriptor		Edge;	// Edge type
typedef graph_traits<Graph>::out_edge_iterator		OutEdgeIt;	// Edge iterator

void testcase() {
	int n, m, u, v;
	cin >> n >> m;
	Graph G(n);
	vector<int> station(n);
	for(int i = 0; i < m; ++i) {
		cin >> u >> v;
		add_edge(u - 1, v - 1, G);
	}
	for(int i = 0; i < n; ++i)
		cin >> station[i];

	// Compute strong components
	vector<int> sccmap(n);	// Use this vector as exterior property map
	int nscc = strong_components(G, make_iterator_property_map(sccmap.begin(), get(vertex_index, G)));

	// For each strong component find cheapest station
	vector<int> cost(nscc, 101);
	for(int i = 0; i < n; ++i) {
		cost[sccmap[i]] = min(cost[sccmap[i]], station[i]);
	}

	// DFS from each component and include encountered components
	vector<bool> visited(n, false);
	for(int i = 0; i < n; ++i) {
		stack<int> S;
		visited[i] = true;
		S.push(i);
		while(!S.empty()) {
			int curr = S.top();
			S.pop();
			OutEdgeIt obit, oeit;
			for(tie(obit, oeit) = out_edges(curr, G); obit != oeit; ++obit) {
				int next = target(*obit, G);
				if(!visited[next]) {
					visited[next] = true;
					S.push(next);
				}
				if(sccmap[curr] != sccmap[next]) {
					cost[sccmap[next]] = 0;
				}
			}
		}
	}

	// Compute total cost
	int sum = 0;
	for(int c : cost) {
		sum += c;
	}

	cout << sum << endl;

}

int main() {
	int t;
	cin >> t;
	while(t--)
		testcase();
}