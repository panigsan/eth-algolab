#include <iostream>
#include <deque>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

//#define WITH_PATHS

using namespace boost;
typedef adjacency_list<vecS,vecS,directedS,
                       no_property,
                       property<edge_weight_t,int>> Graph;
typedef Graph::edge_descriptor Edge;
typedef Graph::vertex_descriptor Vertex;
typedef Graph::edge_iterator EdgeIt;
typedef Graph::vertex_iterator VertexIt;
typedef property_map<Graph,edge_weight_t>::type WeightMap;

int main(){
  int n,m,q; std::cin >> n >> m >> q;

  Graph G(n);
  WeightMap weights = get(edge_weight,G);
  for(int i=0;i<n;i++){
    long lon,lat; std::cin >> lon >> lat;
  }

  for(int i=0;i<m;i++){
    int a,b,c; std::cin >> a >> b >> c;
    Edge e; bool success;
    tie(e,success) = add_edge(a,b,G);
    weights[e] = c;
  }

  for(int i=0;i<q;i++){
    int s,t; std::cin >> s >> t;
    std::vector<Vertex> predecessor(n);
    std::vector<int> distances(n);
    dijkstra_shortest_paths(G,s,
        predecessor_map(make_iterator_property_map(predecessor.begin(),
                                                   get(vertex_index,G))).
        distance_map(make_iterator_property_map(distances.begin(),
                                                get(vertex_index,G))));
    if(distances[t] == std::numeric_limits<int>::max()){
      std::cout << "unreachable" << std::endl;
#ifdef WITH_PATHS
      std::cout << 0 << std::endl;
#endif
    } else {
      std::cout << distances[t] << std::endl;
#ifdef WITH_PATHS
      std::deque<int> sol;

      int last = t;
      while(last != s) {
        sol.push_front(last);
        last = predecessor[last];
      }
      sol.push_front(s);
      std::cout << sol.size() << " ";
      for(int x : sol) std::cout << x << " ";
      std::cout << std::endl;
#endif
    }
  }
}
