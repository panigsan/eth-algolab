#include <iostream>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/edmonds_karp_max_flow.hpp>

#define step(x) //std::cerr << "Step " << x << std::endl
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

void testcase(){
  int l, p;
  std::cin >> l >> p;

  Graph G(l);
  Vertex source = add_vertex(G);
  Vertex target = add_vertex(G);

  EdgeCapacityMap ecm = get(bs::edge_capacity,G);
  ReverseEdgeMap rem = get(bs::edge_reverse,G);
  EdgeAdder ea(G,ecm,rem);

  long goal=0;
  for(int i=0;i<l;++i){
    long g,d;
    std::cin >> g >> d;
    ea.addEdge(source,i,g);
    ea.addEdge(i,target,d);
    goal += d;
  }

  for(int i=0;i<p;++i){
    int u,v;
    std::cin >> u >> v;
    long c_min, c_max;
    std::cin >> c_min >> c_max;
    Vertex w = add_vertex(G);

    // Limit the edge with c_max
    ea.addEdge(u,w,c_max);
    // Force to lose c_min
    ea.addEdge(w,target,c_min);
    goal += c_min;

    ea.addEdge(w,v,c_max-c_min);
    // Get the c_min again
    ea.addEdge(source,v,c_min);
  }

  long flow = bs::push_relabel_max_flow(G,source,target);
  std::cout << (flow == goal ? "yes" : "no") << std::endl;
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int T; std::cin >> T;
  while(T--) testcase();
}
