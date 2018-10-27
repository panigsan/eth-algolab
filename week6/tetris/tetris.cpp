#include <iostream>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/edmonds_karp_max_flow.hpp>

#define step(x) //std::cerr << "Step " << x << std::endl
#define trace(x) std::cerr << #x << " = " << x << std::endl
#define bs boost

typedef bs::adjacency_list_traits<bs::vecS, bs::vecS, bs::directedS> Traits; 
typedef bs::adjacency_list<bs::vecS, bs::vecS, bs::directedS, bs::no_property,
          bs::property<bs::edge_capacity_t, long,
          bs::property<bs::edge_residual_capacity_t, long, 
          bs::property<bs::edge_reverse_t, Traits::edge_descriptor>
        > > > Graph;

typedef bs::property_map<Graph, bs::edge_capacity_t>::type EdgeCapacityMap; 
typedef bs::property_map<Graph, bs::edge_residual_capacity_t>::type ResidualCapacityMap;
typedef bs::property_map<Graph, bs::edge_reverse_t>::type ReverseEdgeMap;
typedef bs::graph_traits<Graph>::vertex_descriptor Vertex;
typedef bs::graph_traits<Graph>::edge_descriptor Edge;

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
		bs::tie(e, success) = bs::add_edge(from, to, G);
		bs::tie(rev_e, success) = bs::add_edge(to, from, G);
		capacitymap[e] = capacity;
		capacitymap[rev_e] = 0; // reverse edge has no capacity!
		revedgemap[e] = rev_e;
		revedgemap[rev_e] = e;
	}
	
};

const long INF = 1<<22;

void testcase(){
  int w,n;
  std::cin >> w >> n;

  Graph G(2*(w+1));

  EdgeCapacityMap ecm = get(bs::edge_capacity,G);
  ReverseEdgeMap rem = get(bs::edge_reverse,G);
  EdgeAdder ea(G,ecm,rem);

  // Each position is represented by two nodes (ending and starting).
  ea.addEdge(0,w+1,INF);
  ea.addEdge(w,2*w+1,INF);

  for(int i=1;i<w;++i){
    ea.addEdge(i,w+i+1,1);
  }

  for(int i=0;i<n;++i){
    int l,r;
    std::cin >> l >> r;
    if(l>r) {
      int t = l;
      l = r;
      r = t;
    }

    Edge e;
    bool success;
    tie(e,success) = edge(w+l+1,r,G);
    // add if not here yet OR it fits the whole width
    if(success && l==0 && r==w) ecm[e]++;
    else ea.addEdge(w+l+1,r,1);

  }
  long flow = push_relabel_max_flow(G,0,2*w+1);
  std::cout << flow << std::endl;
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int T; std::cin >> T;
  while(T--) testcase();
}
