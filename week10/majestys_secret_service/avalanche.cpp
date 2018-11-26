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

#define trace(x) //std::cerr << #x << " = " << x << std::endl

// BGL Graph definitions
// =====================
// Graph Type with nested interior edge properties for Flow Algorithms
typedef	boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> Traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
  boost::property<boost::edge_weight_t,long,
	boost::property<boost::edge_capacity_t, long,
		boost::property<boost::edge_residual_capacity_t, long,
			boost::property<boost::edge_reverse_t, Traits::edge_descriptor> > > >	> Graph;
// Interior Property Maps
typedef	boost::property_map<Graph, boost::edge_weight_t>::type		WeightMap;
typedef	boost::property_map<Graph, boost::edge_capacity_t>::type		EdgeCapacityMap;
typedef	boost::property_map<Graph, boost::edge_residual_capacity_t>::type	ResidualCapacityMap;
typedef	boost::property_map<Graph, boost::edge_reverse_t>::type		ReverseEdgeMap;
typedef	boost::graph_traits<Graph>::vertex_descriptor			Vertex;
typedef	boost::graph_traits<Graph>::edge_descriptor			Edge;
typedef	boost::graph_traits<Graph>::edge_iterator			EdgeIt;


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
  int n,m,a,s,c,d;
  std::cin >> n >> m >> a >> s >> c >> d;
  trace(n);
  trace(m);
  trace(a);
  trace(s);
  trace(c);
  trace(d);
  // 739 1061 69 100 1 1

  Graph G(n);
  WeightMap weights = get(boost::edge_weight,G);
  for(int i=0;i<m;++i){
    char w;
    int x,y,z;
    std::cin >> w >> x >> y >> z;
    Edge e; bool success;
    tie(e,success) = add_edge(x,y,G);
    weights[e] = z;
    if(w=='L'){
      tie(e,success) = add_edge(y,x,G);
      weights[e] = z;
    }
  }

  std::vector<int> n_agents(n,0);
  std::vector<int> n_shelts(n,0);
  for(int i=0;i<a;++i){
    int ag;
    std::cin >> ag;
    ++n_agents[ag];
  }
  for(int i=0;i<s;++i){
    int sh;
    std::cin >> sh;
    ++n_shelts[sh];
  }
  std::vector<std::pair<int,int>> agents;
  std::vector<std::pair<int,int>> shelts;
  for(int i=0;i<n;++i){
    if(n_agents[i] !=0) agents.push_back(std::make_pair(i,n_agents[i]));
    if(n_shelts[i] !=0) shelts.push_back(std::make_pair(i,n_shelts[i]));
  }

  std::vector<std::tuple<long,int,int>> dists;
  long y = 1<<30;
  for(auto ag : agents){
    std::vector<Vertex> predmap(n);	// We will use this vector as an Exterior Property Map: Vertex -> Dijkstra Predecessor
	  std::vector<long> distmap(n,std::numeric_limits<int>::max());		// We will use this vector as an Exterior Property Map: Vertex -> Distance to source
	  Vertex start = ag.first;
	  boost::dijkstra_shortest_paths(G, start, // We MUST provide at least one of the two maps
		  boost::predecessor_map(
		    boost::make_iterator_property_map(predmap.begin(), 
		      boost::get(boost::vertex_index, G))).	// predecessor map as Named Parameter
		  distance_map(boost::make_iterator_property_map(distmap.begin(), 
		      boost::get(boost::vertex_index, G))));	// distance map as Named Parameter
		for(auto sh : shelts){
		  if(distmap[sh.first] < std::numeric_limits<int>::max()){
		    dists.push_back(std::make_tuple(distmap[sh.first],ag.first,sh.first));
		  }
		}
  }

  std::sort(dists.begin(),dists.end());

  long l=0;
  long r=1<<30;
  long q;
  while(l<=r){
    q = l + (r-l)/2;
    Graph G2(n);
    Vertex src = add_vertex(G2);
    Vertex trg = add_vertex(G2);
	  EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G2);
	  ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G2);
	  ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G2);
	  EdgeAdder eaG(G2, capacitymap, revedgemap);
    for(int i=0;i<dists.size();++i){
      if(get<0>(dists[i])>q) break;
      eaG.addEdge(get<1>(dists[i]),get<2>(dists[i]),std::numeric_limits<int>::max());
    }
    for(auto ag : agents){
      eaG.addEdge(src,ag.first,ag.second);
    }
    for(auto sh : shelts){
      eaG.addEdge(sh.first,trg,sh.second);
    }
	  long flow = boost::edmonds_karp_max_flow(G2, src, trg);

    /*
    trace(l);
    trace(q);
    trace(r);
    trace(flow);
    */
    if(flow<a) l = q+1;
    //else if(flow==a) r = q;
    else r = q-1;
  }
  q = l + (r-l)/2;

  std::cout << (q+d) << std::endl;
  /*
	// Create Graph and Maps
	Graph G(4);
	EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
	ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
	ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
	EdgeAdder eaG(G, capacitymap, revedgemap);

	// Add edges
	eaG.addEdge(0, 1, 1); // from, to, capacity
	eaG.addEdge(0, 3, 1);
	eaG.addEdge(2, 1, 1);
	eaG.addEdge(2, 3, 1);

	// Add source and sink
	// Careful: The names 'source' and 'target' are already used for BGL's 
	// functions to get the two endpoints of an edge, use 'src' and 'sink'
	// in case you remove the namespace (which we don't recommend).
	Vertex source = boost::add_vertex(G);
	Vertex target = boost::add_vertex(G);
	eaG.addEdge(source, 0, 2);
	eaG.addEdge(source, 2, 1);
	eaG.addEdge(1, target, 2);
	eaG.addEdge(3, target, 1);

	// Calculate flow
	// If not called otherwise, the flow algorithm uses the interior properties
	// - edge_capacity, edge_reverse (read access),
	// - edge_residual_capacity (read and write access).
	long flow1 = boost::push_relabel_max_flow(G, source, target);
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
