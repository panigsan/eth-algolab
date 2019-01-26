#include <iostream>
#include <cstdlib>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>
// Namespaces
using namespace boost;
using namespace std;

// BGL Graph definitions
// ===================== 
// Graph Type with nested interior edge properties for Cost Flow Algorithms
typedef adjacency_list_traits<vecS, vecS, directedS> Traits;
typedef adjacency_list<vecS, vecS, directedS, no_property,
    property<edge_capacity_t, long,
        property<edge_residual_capacity_t, long,
            property<edge_reverse_t, Traits::edge_descriptor,
                property <edge_weight_t, long> > > > > Graph;
// Interior Property Maps
typedef property_map<Graph, edge_capacity_t>::type      EdgeCapacityMap;
typedef property_map<Graph, edge_weight_t >::type       EdgeWeightMap;
typedef property_map<Graph, edge_residual_capacity_t>::type ResidualCapacityMap;
typedef property_map<Graph, edge_reverse_t>::type       ReverseEdgeMap;
typedef graph_traits<Graph>::vertex_descriptor          Vertex;
typedef graph_traits<Graph>::edge_descriptor            Edge;
typedef graph_traits<Graph>::out_edge_iterator  OutEdgeIt; // Iterator

// Custom Edge Adder Class, that holds the references
// to the graph, capacity map, weight map and reverse edge map
// ===============================================================
class EdgeAdder {
    Graph &G;
    EdgeCapacityMap &capacitymap;
    EdgeWeightMap &weightmap;
    ReverseEdgeMap  &revedgemap;

public:
    EdgeAdder(Graph & G, EdgeCapacityMap &capacitymap, EdgeWeightMap &weightmap, ReverseEdgeMap &revedgemap) 
        : G(G), capacitymap(capacitymap), weightmap(weightmap), revedgemap(revedgemap) {}

    void addEdge(int u, int v, long c, long w) {
        Edge e, reverseE;
        tie(e, tuples::ignore) = add_edge(u, v, G);
        tie(reverseE, tuples::ignore) = add_edge(v, u, G);
        capacitymap[e] = c;
        weightmap[e] = w;
        capacitymap[reverseE] = 0;
        weightmap[reverseE] = -w;
        revedgemap[e] = reverseE; 
        revedgemap[reverseE] = e; 
    }
};

void testcase() {
    int n;
    cin >> n;
    Graph G(2 * n * n);
    EdgeCapacityMap capacitymap = get(edge_capacity, G);
    EdgeWeightMap weightmap = get(edge_weight, G);
    ReverseEdgeMap revedgemap = get(edge_reverse, G);
    ResidualCapacityMap rescapacitymap = get(edge_residual_capacity, G);
    EdgeAdder eaG(G, capacitymap, weightmap, revedgemap);
    int coins, start, end;
    // 2 * (j + n * i) is in vertex, 2 * (j + n * i) is out vertex
    for(int i = 0; i < n; ++i) {
        for(int j = 0; j < n; ++j) {
            cin >> coins;
            if(i == 0 && j == 0)
                start = coins;
            if(i == n - 1 && j == n - 1)
                end = coins;
            // intra-vertex edge
            eaG.addEdge(2 * (j + n * i), 2 * (j + n * i) + 1, 1, 100 - coins);
            // Go down
            if(i < n -1) {
                eaG.addEdge(2 * (j + n * i) + 1, 2 * (j + n * (i + 1)), 1, 0);
            }
            // Go right
            if(j < n - 1) {
                eaG.addEdge(2 * (j + n * i) + 1, 2 * (j + 1 + n * i), 1, 0);
            }
        }
    }

    // compute max-flow-min-cost from first out node to last in node
    // needed to have two flows
    successive_shortest_path_nonnegative_weights(G, 1, 2 * n * n - 2);

    // compute flow cost considering first and last amount of coins
    int max_coins = 100 * 4 * (n - 1) - find_flow_cost(G) + start + end - 200;

    cout << max_coins << endl;

}

int main() {
    int t;
    cin >> t;
    while(t--)
        testcase();
}