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
// Namespaces
// using namespace std;
// using namespace boost;

// BGL Graph definitions
// =====================
// Graph Type with nested interior edge properties for Flow Algorithms
typedef	boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> Traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
	boost::property<boost::edge_capacity_t, int,
		boost::property<boost::edge_residual_capacity_t, int,
			boost::property<boost::edge_reverse_t, Traits::edge_descriptor> > > >	Graph;
// Interior Property Maps
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
  int h,w;
  std::cin >> h >> w;

  std::string note;
  std::cin >> note;

	// Create Graph and Maps
	int letters = 'Z'-'A'+1;
	Graph G(letters+ letters*letters);
	EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
	ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
	ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
	EdgeAdder eaG(G, capacitymap, revedgemap);

	// in case you remove the namespace (which we don't recommend).
	Vertex source = boost::add_vertex(G);
	Vertex target = boost::add_vertex(G);

  std::vector<int> L(letters,0);
	for(char c : note){
	  ++L[c-'A'];
	}
	for(int c=0;c<letters;++c){
	  eaG.addEdge(source,c,L[c]);
	}

  std::vector<std::string> front(h,std::string(w,'0'));
  for(int i=0;i<h;++i){
    std::cin >> front[i];
  }

  std::vector<std::vector<int>> P(letters,std::vector<int>(letters,0));
  for(int i=0;i<h;++i){
    std::string back(w,'0');
    std::cin >> back;
    for(int j=0;j<w;++j){
      char a = front[i][j]-'A';
      char b = back[w-j-1]-'A';
      if(a > b) {
        char t = a;
        a = b;
        b = t;
      }
      ++P[a][b];
    }
  }
  for(int i=0;i<letters;++i){
    for(int j=i;j<letters;++j){
      if(P[i][j]!=0){
        eaG.addEdge(i,letters+i*letters+j,P[i][j]);
        eaG.addEdge(j,letters+i*letters+j,P[i][j]);
        eaG.addEdge(letters+i*letters+j,target,P[i][j]);
      }
    }
  }

	// Calculate flow
	// If not called otherwise, the flow algorithm uses the interior properties
	// - edge_capacity, edge_reverse (read access),
	// - edge_residual_capacity (read and write access).
	long flow = boost::push_relabel_max_flow(G, source, target);
  std::cout << (flow == note.length() ? "Yes" : "No") << std::endl;
}

// Main function to loop over the testcases
int main() {
	std::ios_base::sync_with_stdio(false);
	int T;
  std::cin >> T;
  while(T--) testcase();
}
