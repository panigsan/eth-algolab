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
#include <vector>
#include <unordered_map>
#include <algorithm>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

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

#define trace(x) std::cerr << #x << " = " << x << std::endl
const int INF = 300;
struct book {
  int s;
  int t;
  int d;
  int a;
  int p;
};
void testcase() {
  int N,S;
  std::cin >> N >> S;
  std::vector<int> L(S+1);
  for(int i=1;i<=S;++i) std::cin >> L[i];

  int max_time=0;
  std::vector<book> books(N);
  std::set<int> ad;
  for(int i=0;i<N;++i){
    int s,t,d,a,p;
		std::cin >> s >> t >> d >> a >> p;
    books[i] = {s,t,d,a,p};
    max_time = std::max(max_time,a);
    ad.insert(a);
    ad.insert(d);
  }

  
  std::vector<int> adv;
  for(int x : ad) adv.push_back(x);
  std::sort(adv.begin(),adv.end());

  int n_times = ad.size();
  // Create Graph and Maps
  Graph G(n_times*2);
  EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
  EdgeWeightMap weightmap = boost::get(boost::edge_weight, G);
  ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
  ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
  EdgeAdder eaG(G, capacitymap, weightmap, revedgemap);

  Vertex src = add_vertex(G);
  Vertex trg = add_vertex(G);

  eaG.addEdge(src,0,L[1],0);
  eaG.addEdge(src,n_times,L[2],0);

  for(int i=0;i<n_times-1;++i){
    eaG.addEdge(i,i+1,INF,100);
    eaG.addEdge(n_times+i,n_times+i+1,INF,100);
  }

  for(book b : books){
    int u,v;
    u = std::find(adv.begin(),adv.end(),b.d)-adv.begin();
    v = std::find(adv.begin(),adv.end(),b.a)-adv.begin();
    int dist = v-u;
    if(b.s==2) u += n_times;
    if(b.t==2) v += n_times;
    eaG.addEdge(u,v,1,dist*100-b.p);
  }
  
  eaG.addEdge(n_times-1,trg,INF,0);
  eaG.addEdge(n_times+n_times-1,trg,INF,0);
  boost::successive_shortest_path_nonnegative_weights(G, src, trg);
  int cost2 = boost::find_flow_cost(G);
  std::cout << ((L[1]+L[2])*100*(n_times-1)-cost2) << std::endl; // 12
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int T;
  std::cin >> T;
  while(T--) testcase();
}
