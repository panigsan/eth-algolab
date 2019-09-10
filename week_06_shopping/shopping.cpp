#include <iostream>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

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
  int n,m,s; std::cin >> n >> m >> s;
  Graph G(n);
  EdgeCapacityMap ecm = get(bs::edge_capacity,G);
  ReverseEdgeMap rem = get(bs::edge_reverse,G);
  EdgeAdder ea(G,ecm,rem);

  Vertex source = add_vertex(G);
  Vertex target = add_vertex(G);

  ea.addEdge(source,0,s);

  // Count how many stores are in each intersection
  std::vector<int> stores(n,0);
  for(int i=0;i<s;++i){
    int store; std::cin >> store;
    stores[store]++;
  }
  for(int i=0; i<n;++i){
    if(stores[i]>0)
      ea.addEdge(i,target,stores[i]);
  }

  for(int i=0;i<m;++i){
    int u,v;
    std::cin >> u >> v;
    ea.addEdge(u,v,1);
    ea.addEdge(v,u,1);
  }

  long flow = bs::push_relabel_max_flow(G,source,target);

  std::cout << (flow == s ? "yes" : "no") << std::endl;
}

int main(){
  int T; std::cin >> T;
  while(T--) testcase();
}
