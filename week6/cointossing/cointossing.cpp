#include <iostream>
#include <vector>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

#define bs boost
#define trace(x) //std::cerr << #x << " = " << x << std::endl

typedef bs::adjacency_list<bs::vecS,bs::vecS,bs::directedS> Traits;
typedef bs::adjacency_list<bs::vecS,bs::vecS,bs::directedS,
          bs::no_property,
          bs::property<bs::edge_capacity_t,long,
            bs::property<bs::edge_residual_capacity_t, long,
              bs::property<bs::edge_reverse_t, Traits::edge_descriptor>
          > > > Graph;
typedef Graph::edge_descriptor Edge;
typedef Graph::vertex_descriptor Vertex;
typedef bs::property_map<Graph,bs::edge_capacity_t>::type EdgeCapacityMap;
typedef bs::property_map<Graph,bs::edge_residual_capacity_t>::type EdgeResidualCapacityMap;
typedef bs::property_map<Graph,bs::edge_reverse_t>::type EdgeReverseMap;

class EdgeAdder {
  Graph &G;
  EdgeCapacityMap &capacityMap;
  EdgeReverseMap &reverseMap;

  public:
    EdgeAdder(Graph &G, EdgeCapacityMap &capacityMap, EdgeReverseMap &reverseMap):
      G(G), capacityMap(capacityMap), reverseMap(reverseMap){}
  
    void addEdge(int from, int to, int capacity){
      Edge e, rev_e;
      bool success;
      bs::tie(e,success) = add_edge(from,to,G);
      bs::tie(rev_e,success) = add_edge(to,from,G);
      capacityMap[e] = capacity;
      capacityMap[rev_e] = 0;
      reverseMap[e] = rev_e;
      reverseMap[rev_e] = e;
    }
};

void testcase(){
  int n,m; std::cin >> n >> m;
  std::vector<long> points(n,0);

  Graph G(n);
  Vertex source = add_vertex(G);
  Vertex target = add_vertex(G);
  EdgeCapacityMap ecm = get(bs::edge_capacity,G);
  EdgeReverseMap erm = get(bs::edge_reverse,G);
  EdgeAdder eaG(G,ecm,erm);

  int n_matches = 0;
  for(int i=0;i<m;++i){
    int a,b,c;
    std::cin >> a >> b >> c;
    if(c==1) points[a]++;
    if(c==2) points[b]++;
    if(c==0) {
      Vertex match = add_vertex(G);
      eaG.addEdge(match,a,1);
      eaG.addEdge(match,b,1);
    
      eaG.addEdge(source,match,1);
      
      ++n_matches;
    }
  }

  int goal = 0;
  for(int i=0;i<n;++i){
    long s; std::cin >> s;
    long x = s-points[i];

    eaG.addEdge(i,target,x);
    goal += x;
    }
  }

  long flow = bs::push_relabel_max_flow(G,source,target);

  if(flow == n_matches && goal == flow)
    std::cout << "yes" << std::endl;
  else
    std::cout << "no" << std::endl;



}
int main(){
  std::ios_base::sync_with_stdio(false);
  int T; std::cin >> T;
  while(T--) testcase();
}
