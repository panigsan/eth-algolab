#include <iostream>
#include <algorithm>
#include <limits>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/strong_components.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

// Used for debugging
#define step(x) //std::cout << "Step (" << x << ")" << std::endl
#define trace(x) //std::cout << #x << " = " << x << std::endl

using namespace boost;

typedef adjacency_list<vecS,vecS,directedS,
                       no_property,
                       property<edge_weight_t,long>> Graph;

typedef Graph::edge_descriptor Edge;
typedef Graph::vertex_descriptor Vertex;
typedef Graph::edge_iterator EdgeIt;
typedef Graph::vertex_iterator VertexIt;
typedef property_map<Graph,edge_weight_t>::type WeightMap;


// Idea:
// Create a graph with inverse edges since we care about only one target 
// (which can be used as source). Run strong components to get all nodes
// which are in a cycle (teleport network). For each of these components, 
// create a central node where the nodes which belong to this component are attached.
void testcase(){
  int n,m,k,T;
  std::cin >> n >> m >> k >> T;
  std::vector<bool> tele(n,false);

  for(int i=0;i<T;++i){
    int ti; std::cin >> ti;
    tele[ti] = true;
  }

  // The graph will contain the nodes and the central nodes of the tele network
  Graph G(2*n);
  WeightMap weights = get(edge_weight,G);
  
  for(int i=0;i<m;++i){
    Edge e; bool success;
    int u,v,c;
    std::cin >> u >> v >> c;
    tie(e,success) = add_edge(v,u,G);
    weights[e] = c;
  }


  step("strong");
  std::vector<int> sccmap(2*n);
  int nscc = strong_components(G,
                make_iterator_property_map(sccmap.begin(),get(vertex_index,G)));
  std::vector<std::vector<int>> connected(nscc);

  step("connected");
  for(int i=0;i<n;++i){
    if(tele[i]){
      connected[sccmap[i]].push_back(i);
    }
  }

  // Withouth using central nodes this would take too long
  step("add new edges");
  trace(connected.size());
  for(int i=0; i < connected.size(); ++i){
    if(connected[i].size() > 1){
      for(int x : connected[i]){
        Edge e; bool success;
        tie(e,success) = add_edge(x,n+i,G);
        weights[e] = 0;
        tie(e,success) = add_edge(n+i,x,G);
        weights[e] = connected[i].size()-1;
      }
    }
  }

  step("dijkstra");
  long best = std::numeric_limits<long>::max();
  std::vector<Vertex> predecessor(2*n);
  std::vector<long> distances(2*n);
  dijkstra_shortest_paths(G,n-1,
        predecessor_map(make_iterator_property_map(predecessor.begin(),
                                                        get(vertex_index,G))).
          distance_map(make_iterator_property_map(distances.begin(),
                                                  get(vertex_index,G))));

  for(int i=0;i<k;++i){
    best = std::min(best,distances[i]);
  }

  if(best <= 1000000)
    std::cout << best << std::endl;
  else
    std::cout << "no" << std::endl;


}
  
int main(){
  int t; std::cin >> t;
  while(t--) testcase();
}
