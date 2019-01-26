
// Includes
// ========
// STL includes
#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <map>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/edmonds_karp_max_flow.hpp>

#define trace(x) std::cerr << #x << " = " << x << std::endl

// BGL Graph definitions
// =====================
// Graph Type with nested interior edge properties for Flow Algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> Traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
  boost::property<boost::edge_capacity_t, long,
    boost::property<boost::edge_residual_capacity_t, long,
      boost::property<boost::edge_reverse_t, Traits::edge_descriptor> > > > Graph;
// Interior Property Maps
typedef boost::property_map<Graph, boost::edge_capacity_t>::type    EdgeCapacityMap;
typedef boost::property_map<Graph, boost::edge_residual_capacity_t>::type ResidualCapacityMap;
typedef boost::property_map<Graph, boost::edge_reverse_t>::type   ReverseEdgeMap;
typedef boost::graph_traits<Graph>::vertex_descriptor     Vertex;
typedef boost::graph_traits<Graph>::edge_descriptor     Edge;
typedef boost::graph_traits<Graph>::edge_iterator     EdgeIt;


// Custom Edge Adder Class, that holds the references
// to the graph, capacity map and reverse edge map
// ===================================================
class EdgeAdder {
  Graph &G;
  EdgeCapacityMap &capacitymap;
  ReverseEdgeMap  &revedgemap;

public:
  // to initialize the Object
  EdgeAdder(Graph & G, EdgeCapacityMap &capacitymap, ReverseEdgeMap &revedgemap):
    G(G), capacitymap(capacitymap), revedgemap(revedgemap){}

  // to use the Function (add an edge)
  Edge addEdge(int from, int to, long capacity) {
    Edge e, rev_e;
    bool success;
    boost::tie(e, success) = boost::add_edge(from, to, G);
    boost::tie(rev_e, success) = boost::add_edge(to, from, G);
    capacitymap[e] = capacity;
    capacitymap[rev_e] = 0; // reverse edge has no capacity!
    revedgemap[e] = rev_e;
    revedgemap[rev_e] = e;
    return e;
  }
};

const long INF = 100000;

struct Info{
  long id;
  long count;
};

void testcase() {
  int n,m;
  std::cin >> n >> m;

  std::map<long,Info> strengths;
  std::map<long,Info> weights;
  long max_strength=0;
  int idx=0;
  for(int i=0;i<n;++i){
    long s;
    std::cin >> s;
    max_strength = std::max(max_strength,s);
    auto it = strengths.find(s);
    if(it==strengths.end()){
      strengths.insert({s,{idx,1}});
      ++idx;
    } else {
      ++(it->second.count);
    }
  }

  long max_weight=0;
  for(int i=0;i<m;++i){
    long w;
    std::cin >> w;
    max_weight = std::max(max_weight,w);
    auto it = weights.find(w);
    if(it==weights.end()){
      weights.insert({w,{idx,1}});
      ++idx;
    } else {
      ++(it->second.count);
    }
  }

  if(max_weight > max_strength){
    std::cout << "impossible" << std::endl;
    return;
  }
  Graph G(idx);
  Vertex src = boost::add_vertex(G);
  Vertex trg = boost::add_vertex(G);
  EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
  ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
  ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
  EdgeAdder eaG(G, capacitymap, revedgemap);

  for(auto pr : strengths){
    eaG.addEdge(src,pr.second.id,3);
    auto it = weights.lower_bound(pr.first);
    if(it != weights.end() && it->first <= pr.first)
      eaG.addEdge(pr.second.id,it->second.id,INF);
  }

  for(auto pr : weights){
    auto it = weights.lower_bound(pr.first-1);
    if(it!=weights.end() && pr.first >= it->first){
      eaG.addEdge(pr.second.id,it->second.id,INF);
    }
    eaG.addEdge(pr.second.id,trg,pr.second.count);
  }
  trace(1);

  long l=1;
  long r=std::max(n,m);
  long c;
  while(l<r){
    c = (l+r)/2;
    for(auto pr : strengths) {
      Edge e; bool success;
      tie(e,success) = boost::edge(src,pr.second.id,G);
      capacitymap[e] = c*pr.second.count;
    }
    //for(int i=0;i<n;++i) capacitymap[edges_to_person[i]] = c;
    //long flow1 = boost::edmonds_karp_max_flow(G, src, trg);
    long flow1 = boost::push_relabel_max_flow(G, src, trg);
    if(flow1<m) l=c+1;
    else r = c;
  }

  c = (l+r)/2;
  
  trace(c);
  long sol = 3*c-1;
  std::cout << sol << std::endl;
}

// Main function to loop over the testcases
int main() {
  std::ios_base::sync_with_stdio(false);
  int T;
  std::cin >> T;
  while(T--) testcase();
}
