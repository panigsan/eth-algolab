#include <iostream>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

using namespace boost;
typedef adjacency_list<vecS, vecS, undirectedS,
                       no_property,
                       property<edge_weight_t, int>> Graph;
typedef Graph::edge_descriptor Edge;
typedef Graph::vertex_descriptor Vertex;
typedef Graph::edge_iterator EdgeIt;
typedef property_map<Graph, edge_weight_t>::type WeightMap;

void testcase(){
  int n, m;
  std::cin >> n >> m;

  // init
  Graph G(n);
  WeightMap weights = get(edge_weight, G);

  for(int i=0;i<m;++i){
    int u,v,w;
    std::cin >> u >> v >> w;
    bool added;
    Edge e;
    tie(e,added) = add_edge(u,v,G);
    weights[e] = w;
  }

  // mst
  std::vector<Edge> mst;
  kruskal_minimum_spanning_tree(G,std::back_inserter(mst));

  int W=0;
  for(Edge e : mst){
    W += weights[e];
  }

  // shortest path to furthest point
  std::vector<Vertex> predmap(n);
  std::vector<int> distmap(n);
  
  Vertex start = 0;
  dijkstra_shortest_paths(G, start,
      predecessor_map(make_iterator_property_map(predmap.begin(),
                                                 get(vertex_index,G))).
        distance_map(make_iterator_property_map(distmap.begin(),
                                                get(vertex_index,G))));

  int maxdist = 0;
  for(int d : distmap){
    maxdist = std::max(maxdist,d);
  }

  std::cout << W << " " << maxdist << std::endl;
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  while(t--) testcase();
}
