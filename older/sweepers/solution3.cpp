// Week 13 - Sweepers
#include <iostream>
#include <vector>
#include <queue>
#include <set>
#include <algorithm>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/strong_components.hpp>
// Namespaces
using namespace std;
using namespace boost;


// BGL Graph definitions
// =====================
// Graph Type with nested interior edge properties for Flow Algorithms
typedef adjacency_list<vecS, vecS, undirectedS, no_property, no_property> Graph2;
typedef graph_traits<Graph2>::out_edge_iterator      OutEdgeIt2;

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
typedef graph_traits<Graph>::out_edge_iterator      OutEdgeIt;
typedef graph_traits<Graph>::edge_iterator      EdgeIt;

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
    int n, m, s, pos, u, v;
    cin >> n >> m >> s;
    Graph G(n);
    Vertex source = add_vertex(G), sink = add_vertex(G);
	EdgeCapacityMap capacitymap = get(edge_capacity, G);
	ReverseEdgeMap revedgemap = get(edge_reverse, G);
	ResidualCapacityMap rescapacitymap = get(edge_residual_capacity, G);
	EdgeAdder eaG(G, capacitymap, revedgemap);
    Graph2 G2(n);
    vector<int> in(n, 0), out(n, 0);
	// Get input and build graph
    for(int i = 0; i < s; ++i) {
        cin >> pos;
        eaG.addEdge(source, pos, 1);
        in[pos]++;
    }
    for(int i = 0; i < s; ++i) {
        cin >> pos;
        eaG.addEdge(pos, sink, 1);
        out[pos]++;
    }
    for(int i = 0; i < m; ++i) {
        cin >> u >> v;
        eaG.addEdge(u, v, 1);
        eaG.addEdge(v, u, 1);
        add_edge(u, v, G2);
    }

    // BFS to check that all corridors are reachable from starting positions
    vector<bool> vvisited(n, false);
    std::set<pair<int,int> > evisited;
    std::queue<int> waiting;
    waiting.push(0);
    while(!waiting.empty()) {
        int curr = waiting.front();
        waiting.pop();
        OutEdgeIt2 oebeg, oeend;
        for(tie(oebeg, oeend) = out_edges(curr, G2); oebeg != oeend; ++oebeg) {
            int next = target(*oebeg, G2);
            //cout << "next " << next << endl;
            if(evisited.find(make_pair(curr, next)) == evisited.end()) {
                evisited.insert(make_pair(curr, next));
            }
            if(!vvisited[next]) {
                vvisited[next] = true;
                waiting.push(next);
            }
        }
    }

    bool possible = evisited.size() == 2 * m;
    
    // Check that all rooms have an even degree
    if(possible) {
        for(u = 0; u < n; ++u) {
            OutEdgeIt oebeg, oeend;
            tie(oebeg, oeend) = out_edges(u, G);
            int degree = (oeend - oebeg) / 2 + in[u] + out[u];
            if(degree % 2 != 0) {
                possible = false;
                break;
            }
        }
    }

    // Find and check that max flow equal s
    if(possible && push_relabel_max_flow(G, source, sink) != s)
        possible = false;

    // Output
    if(possible)
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