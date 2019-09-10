#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <algorithm>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/prim_minimum_spanning_tree.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/connected_components.hpp>
#include <boost/graph/max_cardinality_matching.hpp>


// BGL Graph definitions
// =====================
// Graph Type, OutEdgeList Type, VertexList Type, (un)directedS
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,   
  boost::no_property,
    boost::property<boost::edge_weight_t, long>
    > Graph;
typedef boost::graph_traits<Graph>::edge_descriptor   Edge;
typedef boost::graph_traits<Graph>::vertex_descriptor   Vertex;
typedef boost::graph_traits<Graph>::edge_iterator   EdgeIt;
typedef boost::graph_traits<Graph>::out_edge_iterator   OutEdgeIt;
typedef boost::property_map<Graph, boost::edge_weight_t>::type  WeightMap;

using namespace std;

void testcase(){
  int n,i;
  cin >> n >> i;
  
  Graph G(n);
  WeightMap weightmap = get(boost::edge_weight, G);
  for(int j=0;j<n-1;++j){
    for(int k=j+1;k<n;++k){
      long c;
      cin >> c;
      Edge e; bool success;
      tie(e,success) = boost::add_edge(j,k,G);
      weightmap[e]=c;
    }
  }

  std::vector<Vertex> primpredmap(n);
  boost::prim_minimum_spanning_tree(G, boost::make_iterator_property_map(primpredmap.begin(), boost::get(boost::vertex_index, G)));
  
  Graph mst(n);
  WeightMap weightmap_mst = get(boost::edge_weight, mst);
  long tot=0;
  for(int j=0;j<n;++j){
    if(primpredmap[j]!=j){
      Edge e; bool success;
      tie(e,success) = boost::edge(j,primpredmap[j],G);
      long cost = weightmap[e];
      weightmap[e]=-1; // store that this edge belongs to mst
      tie(e,success) = boost::add_edge(j,primpredmap[j],mst);
      weightmap_mst[e]=cost;
      tot += cost;
    } 
  }

  std::vector<std::vector<long>> DP(n,std::vector<long>(n,0));
  for(int j=0;j<n;++j){
    std::queue<int> Q;
    std::vector<bool> visited(n,false);
    Q.push(j);
    while(!Q.empty()){
      int next = Q.front(); Q.pop();
      visited[next] = true;
      OutEdgeIt ebeg, eend;
      for(boost::tie(ebeg,eend) = boost::out_edges(next,mst);ebeg!=eend;++ebeg){
        Vertex v = boost::target(*ebeg,mst);
        if(!visited[v]){
          Q.push(v);
          DP[j][v] = std::max(DP[j][next], weightmap_mst[*ebeg]);
        }
      }
    }
  }
  
  long best_diff=std::numeric_limits<long>::max();
  EdgeIt ebeg,eend;
  for(tie(ebeg,eend) = boost::edges(G);ebeg!=eend;++ebeg){
    if(weightmap[*ebeg]!=-1){
      long diff = weightmap[*ebeg]-DP[boost::target(*ebeg,G)][boost::source(*ebeg,G)];
      best_diff = std::min(best_diff,diff);
    }
  }

  std::cout << tot + best_diff << std::endl;

}

int main(){
  ios_base::sync_with_stdio(false);
  int T;
  cin >> T;
  while(T--) testcase();
}
