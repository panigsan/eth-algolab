#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/max_cardinality_matching.hpp>

using namespace boost;

typedef adjacency_list<vecS,vecS,undirectedS,
                       no_property,
                       property<edge_weight_t,int>> Graph;
typedef Graph::vertex_descriptor Vertex;
typedef Graph::edge_descriptor Edge;
typedef Graph::edge_iterator EdgeIt;
typedef property_map<Graph,edge_weight_t>::type WeightMap;

void testcase(){
  int n,c,f;
  std::cin >> n >> c >> f;


  Graph G1(n);
  WeightMap weights = get(edge_weight,G1);

  // stores who has a specific preference for quick access
  std::vector<std::vector<int>> who_has(c*n,std::vector<int>(0));
  std::map<std::string,int> mapping;
  int mapping_idx=0;
  for(int i=0;i<n;i++){
    for(int j=0;j<c;j++){
      std::string s; std::cin >> s;
      if(mapping.find(s)==mapping.end()){
        mapping.insert(std::make_pair(s,mapping_idx));
        mapping_idx++;
      } 
      int s_idx = mapping.find(s)->second;

      Edge e; bool success;
      for(int k : who_has[s_idx]){
          tie(e,success) = edge(i,k,G1);
          // add edge only if it does not exist yet
          if(!success){
            tie(e,success) = add_edge(i,k,G1);
          }
          weights[e]++;
      }
      who_has[s_idx].push_back(i);
    }
  }

  Graph G2(n);
  
  EdgeIt ebeg, eend;
  for(tie(ebeg,eend) = edges(G1);ebeg!=eend;++ebeg){
    Edge e = *ebeg;
    if(weights[e] > f){
      add_edge(source(e,G1),target(e,G1),G2);
    }
  }

  // Invoke matching
  std::vector<Vertex> matemap(n);

  edmonds_maximum_cardinality_matching(G2,
      make_iterator_property_map(matemap.begin(),get(vertex_index,G2)));

  int matchsize = matching_size(G2,
      make_iterator_property_map(matemap.begin(),get(vertex_index,G2)));

  if(matchsize == n/2)
    std::cout << "not optimal" << std::endl;
  else
    std::cout << "optimal" << std::endl;

}

int main(){
  std::ios_base::sync_with_stdio(false);
  int T; std::cin >> T;
  while(T--) testcase();
}
