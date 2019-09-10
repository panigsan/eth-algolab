#include <iostream>
#include <vector>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/prim_minimum_spanning_tree.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

using namespace boost;

typedef boost::adjacency_list<vecS,vecS,undirectedS,
               no_property, // vertex property
               property<edge_weight_t,int>> Graph;
typedef Graph::edge_descriptor Edge;
typedef Graph::vertex_descriptor Vertex;
typedef Graph::edge_iterator EdgeIt;
typedef property_map<Graph, edge_weight_t>::type WeightMap;

void testcase(){
  int n,e,s,a,b;
  std::cin >> n >> e >> s >> a >> b;

  // public global network
  Graph G(n);
  WeightMap weights = get(edge_weight,G);

  // private networks
  std::vector<Graph> VG(s,Graph(n));
  std::vector<WeightMap> Vweights;
  Vweights.reserve(s);
  for(Graph pG : VG){
    Vweights.push_back(get(edge_weight,pG));
  }
  for(int i=0;i<e;i++){
    int t1, t2;
    std::cin >> t1 >> t2;
    Edge e; bool success;
    tie(e,success) = add_edge(t1,t2,G);
    weights[e] = std::numeric_limits<int>::max();

    for(int j=0;j<s;j++){
      int w; std::cin >> w;
      tie(e,success) = add_edge(t1,t2,VG[j]);
      Vweights[s][e] = w;
    }
  }

  // prim algorithm for each private graph
  // and updated global network
  for(int i=0;i<s;i++){
    int h; std::cin >> h;
    std::vector<Vertex> primpredmap(n);
    
    prim_minimum_spanning_tree(VG[i],
        make_iterator_property_map(primpredmap.begin(),
                                   get(vertex_index,VG[i])),
        root_vertex(h));

    for(int j=0;j<n;j++){
      if(primpredmap[j] != j){
        Edge e; bool success;
        tie(e,success) = edge(j,primpredmap[j],VG[i]);

        // find the same edge in the global network
        Vertex u = source(e,VG[i]);
        Vertex v = target(e,VG[i]);
        Edge e_ = edge(u,v,G).first;
        weights[e_] = std::min(weights[e_],Vweights[i][e]);
      }
    }
  }

  // shortest path in global network
  std::vector<Vertex> predmap(n);
  std::vector<int> distmap(n);
  Vertex start = a;
  dijkstra_shortest_paths(G,start,
      predecessor_map(make_iterator_property_map(predmap.begin(),
                                                 get(vertex_index,G))).
      distance_map(make_iterator_property_map(distmap.begin(),get(vertex_index,G))));

  std::cout << distmap[b] << std::endl;
}

int main(){
  int T; std::cin >> T;
  while(T--) testcase();
}
