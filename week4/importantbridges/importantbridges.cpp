#include <iostream>
#include <vector>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/undirected_dfs.hpp>
#include <boost/graph/biconnected_components.hpp>

//#include <boost/graph/depth_first_search.hpp>
using namespace boost;
namespace boost
{
  struct edge_component_t
  {
    enum
    { num = 555 };
    typedef edge_property_tag kind;
  }
  edge_component;
}
typedef adjacency_list<vecS,vecS,undirectedS,
                       no_property,
                       //property<vertex_info_t, VertexInfo>,
                       property<edge_component_t, int> > Graph;
typedef Graph::edge_descriptor Edge;
typedef Graph::vertex_descriptor Vertex;
typedef Graph::edge_iterator EdgeIt;
typedef Graph::out_edge_iterator OutEdgeIt;
typedef Graph::vertex_iterator VertexIt;

void testcase(){
  int n,m;
  std::cin >> n >> m;

  Graph G(n);

  for(int i=0;i<m;i++){
    int u,v;
    std::cin >> u >> v;
    add_edge(u,v,G);
  }
  

  auto component = get(edge_component,G);
  int n_bi = biconnected_components(G, component);

  std::vector<int> cnt(n_bi,0);

  EdgeIt ebeg,eend;
  for(tie(ebeg,eend) = edges(G);ebeg!=eend;++ebeg){
    cnt[component[*ebeg]]++;
  }

  std::vector<std::pair<int,int>> res;
  for(tie(ebeg,eend) = edges(G);ebeg!=eend;++ebeg){
    if(cnt[component[*ebeg]]==1){
      int u = source(*ebeg,G);
      int v = target(*ebeg,G);
      res.push_back(std::make_pair(std::min(u,v),std::max(u,v)));
    }
  }
  
  std::sort(res.begin(),res.end());
  std::cout << res.size() << std::endl;
  for(auto p : res){
    std::cout << p.first << " " << p.second << std::endl;
  }

}

int main(){
  int T; std::cin >> T;
  while(T--) testcase();
}
