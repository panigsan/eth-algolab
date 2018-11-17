// ALGOLAB BGL Tutorial 3
// Code snippets demonstrating 
// - MinCostMaxFlow with arbitrary edge costs using cycle_canceling
// - MinCostMaxFlow with non-negative edge costs using successive_shortest_path_nonnegative_weights

// Compile and run with one of the following:
// g++ -std=c++11 -O2 bgl_mincostmaxflow.cpp -o bgl_mincostmaxflow; ./bgl_mincostmaxflow
// g++ -std=c++11 -O2 -I path/to/boost_1_58_0 bgl_mincostmaxflow.cpp -o bgl_mincostmaxflow; ./bgl_mincostmaxflow

// Includes
// ========
// STL includes
#include <iostream>
#include <cstdlib>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

#define step(x) std::cerr << "Step " << x << std::endl

// BGL Graph definitions
// ===================== 
// Graph Type with nested interior edge properties for Cost Flow Algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> Traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, Traits::edge_descriptor,
                boost::property <boost::edge_weight_t, long> > > > > Graph; // new!
// Interior Property Maps
typedef boost::property_map<Graph, boost::edge_capacity_t>::type      EdgeCapacityMap;
typedef boost::property_map<Graph, boost::edge_weight_t >::type       EdgeWeightMap; // new!
typedef boost::property_map<Graph, boost::edge_residual_capacity_t>::type ResidualCapacityMap;
typedef boost::property_map<Graph, boost::edge_reverse_t>::type       ReverseEdgeMap;
typedef boost::graph_traits<Graph>::vertex_descriptor          Vertex;
typedef boost::graph_traits<Graph>::edge_descriptor            Edge;
typedef boost::graph_traits<Graph>::out_edge_iterator  OutEdgeIt; // Iterator

// Custom Edge Adder Class, that holds the references
// to the graph, capacity map, weight map and reverse edge map
// ===============================================================
class EdgeAdder {
    Graph &G;
    EdgeCapacityMap &capacitymap;
    EdgeWeightMap &weightmap;
    ReverseEdgeMap  &revedgemap;

public:
    EdgeAdder(Graph &G, EdgeCapacityMap &capacitymap, EdgeWeightMap &weightmap, ReverseEdgeMap &revedgemap) 
        : G(G), capacitymap(capacitymap), weightmap(weightmap), revedgemap(revedgemap) {}

    void addEdge(int u, int v, long c, long w) {
        Edge e, rev_e;
        boost::tie(e, boost::tuples::ignore) = boost::add_edge(u, v, G);
        boost::tie(rev_e, boost::tuples::ignore) = boost::add_edge(v, u, G);
        capacitymap[e] = c;
        weightmap[e] = w; // new!
        capacitymap[rev_e] = 0;
        weightmap[rev_e] = -w; // new
        revedgemap[e] = rev_e; 
        revedgemap[rev_e] = e; 
    }
};


const int INF = 1<<20;
void testcase(){
  int n;
  std::cin >> n;

  // Create Graph and Maps
  Graph G(n);
  EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
  EdgeWeightMap weightmap = boost::get(boost::edge_weight, G);
  ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
  ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
  EdgeAdder eaG(G, capacitymap, weightmap, revedgemap);
  
  Vertex src = add_vertex(G);
  Vertex trg = add_vertex(G);

  for(int i=0;i<n;++i){
    int a,c;
    std::cin >> a >> c;
    /*
    eaG.addEdge(src,2*i,a,0);
    eaG.addEdge(2*i,2*i+1,a,c);
    */
    eaG.addEdge(src,i,a,c);
  }

  int goal=0;
  for(int i=0;i<n;++i){
    int s,p;
    std::cin >> s >> p;
    goal += s;
    //eaG.addEdge(2*i+1,trg,s,-p);
    eaG.addEdge(i,trg,s,-p);
  }
  
  for(int i=0;i<n-1;++i){
    int v,e;
    std::cin >> v >> e;
    eaG.addEdge(i,i+1,v,e);
    //eaG.addEdge(2*i+1,2*(i+1)+1,v,e);
  }
  // Run the algorithm

  // Option 1: Min Cost Max Flow with cycle_canceling
  step("flow");
  int flow1 = boost::push_relabel_max_flow(G, src, trg);
  step("cycle");
  boost::cycle_canceling(G);
  step("find");
  int cost1 = boost::find_flow_cost(G);

  /*
  int gains = 0;
  int flow2 = 0;
  for(int i=0;i<n;++i){
    Edge e; bool success;
    tie(e,success) = edge(2*i+1,trg,G);
    flow2 += (capacitymap[e]-rescapacitymap[e]);
    gains += P[i]*(capacitymap[e]-rescapacitymap[e]);
  }
  */

  std::cout << (flow1 == goal ? "possible" : "impossible") << " "
            << flow1 << " "
            << -cost1 << std::endl;

  /*
  // Option 2: Min Cost Max Flow with successive_shortest_path_nonnegative_weights
  boost::successive_shortest_path_nonnegative_weights(G, v_source, v_target);
  int cost2 = boost::find_flow_cost(G);
  std::cout << "-----------------------" << std::endl;
  std::cout << "Minimum Cost Maximum Flow with successive_shortest_path_nonnegative_weights()" << std::endl;
  std::cout << "cost " << cost2 << std::endl; // 12
  int s_flow = 0;
  // Iterate over all edges leaving the source to sum up the flow values.
  OutEdgeIt e, eend;
  for(boost::tie(e, eend) = boost::out_edges(boost::vertex(v_source,G), G); e != eend; ++e) {
      std::cout << "edge from " << boost::source(*e, G) << " to " << boost::target(*e, G) 
          << " with capacity " << capacitymap[*e] << " and residual capacity " << rescapacitymap[*e] << std::endl;
      s_flow += capacitymap[*e] - rescapacitymap[*e];
  }
  std::cout << "s-out flow " << s_flow << std::endl; // 5
  // Or equivalently, you can do the summation at the sink, but with reversed sign.
  int t_flow = 0;
  for(boost::tie(e, eend) = boost::out_edges(boost::vertex(v_target,G), G); e != eend; ++e) {
      std::cout << "edge from " << boost::source(*e, G) << " to " << boost::target(*e, G) 
          << " with capacity " << capacitymap[*e] << " and residual capacity " << rescapacitymap[*e] << std::endl;
      t_flow += rescapacitymap[*e] - capacitymap[*e];
  }
  */

}

int main() {
  std::ios_base::sync_with_stdio(false);
  int T;
  std::cin >> T;
  while(T--) testcase();
  return 0;
}
