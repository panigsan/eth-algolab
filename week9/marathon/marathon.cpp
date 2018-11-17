// ALGOLAB BGL Tutorial 2
// Flow example demonstrating
// - interior graph properties for flow algorithms
// - custom edge adder

// Compile and run with one of the following:
// g++ -std=c++11 -O2 flows.cpp -o flows ./flows
// g++ -std=c++11 -O2 -I path/to/boost_1_58_0 flows.cpp -o flows; ./flows

// Includes
// ========
// STL includes
#include <iostream>
#include <vector>
#include <algorithm>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/edmonds_karp_max_flow.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
// Namespaces
// using namespace std;
// using namespace boost;


// BGL Graph definitions
// =====================
// Graph Type with nested interior edge properties for Flow Algorithms
typedef	boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> Traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
  boost::property<boost::edge_weight_t,int,
	boost::property<boost::edge_capacity_t, long,
		boost::property<boost::edge_residual_capacity_t, long,
			boost::property<boost::edge_reverse_t, Traits::edge_descriptor> > > >	> Graph;
// Interior Property Maps
typedef	boost::property_map<Graph, boost::edge_capacity_t>::type		EdgeCapacityMap;
typedef	boost::property_map<Graph, boost::edge_residual_capacity_t>::type	ResidualCapacityMap;
typedef	boost::property_map<Graph, boost::edge_reverse_t>::type		ReverseEdgeMap;
typedef	boost::graph_traits<Graph>::vertex_descriptor			Vertex;
typedef	boost::graph_traits<Graph>::edge_descriptor			Edge;
typedef	boost::graph_traits<Graph>::edge_iterator			EdgeIt;
typedef boost::property_map<Graph, boost::edge_weight_t>::type	WeightMap;


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
		Edge e, rev_e;
		bool success;
		boost::tie(e, success) = boost::add_edge(from, to, G);
		boost::tie(rev_e, success) = boost::add_edge(to, from, G);
		capacitymap[e] = capacity;
		capacitymap[rev_e] = 0; // reverse edge has no capacity!
		revedgemap[e] = rev_e;
		revedgemap[rev_e] = e;
	}
};


// Functions
// =========
// Function for an individual testcase
void testcase() {

  int n,m,s,f;
  std::cin >> n >> m >> s >> f;

	// Create Graph and Maps
	Graph G(n);
	WeightMap weightmap = boost::get(boost::edge_weight, G);
	EdgeCapacityMap capacitymap1 = boost::get(boost::edge_capacity, G);

  std::vector<int> C(n);
  for(int i=0;i<m;++i){
    int a,b,c,d;
    std::cin >> a >> b >> c >> d;
    Edge e; bool success;
    tie(e,success) = add_edge(a,b,G);
    weightmap[e] = d;
    capacitymap1[e] = c;
    tie(e,success) = add_edge(b,a,G);
    weightmap[e] = d;
    capacitymap1[e] = c;
  }

  // Dijkstra
  std::vector<Vertex> predmapS(n);
  std::vector<int> distmapS(n);
  boost::dijkstra_shortest_paths(G, s, // We MUST provide at least one of the two maps
		boost::predecessor_map(boost::make_iterator_property_map(predmapS.begin(), 
		    boost::get(boost::vertex_index, G))).	// predecessor map as Named Parameter
		distance_map(boost::make_iterator_property_map(distmapS.begin(), 
		    boost::get(boost::vertex_index, G))));

  std::vector<Vertex> predmapT(n);
  std::vector<int> distmapT(n);
  boost::dijkstra_shortest_paths(G, f, // We MUST provide at least one of the two maps
		boost::predecessor_map(boost::make_iterator_property_map(predmapT.begin(), 
		    boost::get(boost::vertex_index, G))).	// predecessor map as Named Parameter
		distance_map(boost::make_iterator_property_map(distmapT.begin(), 
		    boost::get(boost::vertex_index, G))));

	int goal = distmapS[f];
	
	Graph G2(n);

	EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G2);
	ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G2);
	ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G2);
	EdgeAdder eaG(G2, capacitymap, revedgemap);

	EdgeIt ebeg, eend;
	for(tie(ebeg,eend) = edges(G); ebeg!=eend; ++ebeg){
	  if(distmapS[source(*ebeg,G)] + weightmap[*ebeg] + distmapT[target(*ebeg,G)] == goal){
	    eaG.addEdge(source(*ebeg,G),target(*ebeg,G),capacitymap1[*ebeg]);
	  }
	}

	long flow1 = boost::push_relabel_max_flow(G2, s, f);
  std::cout << flow1 << std::endl;



	// If not called otherwise, the flow algorithm uses the interior properties
	// - edge_capacity, edge_reverse (read access),
	// - edge_residual_capacity (read and write access).
	/*
	long flow2 = boost::edmonds_karp_max_flow(G, source, target);
	std::cout << "(push relabel max flow) " << flow1 << " == " << flow2 << " (Edmonds Karp max flow)" << std::endl;

	// Iterate over all the edges to print the flow along them
	EdgeIt ebeg, eend;
	for (tie(ebeg, eend) = edges(G); ebeg != eend; ++ebeg) {
		std::cout << "edge from " << boost::source(*ebeg, G) << " to " << boost::target(*ebeg, G) 
				  << " runs " << capacitymap[*ebeg] - rescapacitymap[*ebeg]
                                  << " units of flow (negative for reverse direction)." << std::endl;
	}
	*/

}

// Main function to loop over the testcases
int main() {
	std::ios_base::sync_with_stdio(false);
	int T;
  std::cin >> T;
	while(T--) testcase();
	return 0;
}
