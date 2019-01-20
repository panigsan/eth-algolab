#include <iostream>
#include <vector>
#include <algorithm>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/edmonds_karp_max_flow.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

#define trace(x) //std::cerr << #x << " = " << x << std::endl

// BGL Graph definitions
// =====================
// Graph Type with nested interior edge properties for Flow Algorithms
typedef	boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> Traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
  boost::property<boost::edge_weight_t,long,
	boost::property<boost::edge_capacity_t, long,
		boost::property<boost::edge_residual_capacity_t, long,
			boost::property<boost::edge_reverse_t, Traits::edge_descriptor> > > >	> Graph;
// Interior Property Maps
typedef	boost::property_map<Graph, boost::edge_weight_t>::type		WeightMap;
typedef	boost::property_map<Graph, boost::edge_capacity_t>::type		EdgeCapacityMap;
typedef	boost::property_map<Graph, boost::edge_residual_capacity_t>::type	ResidualCapacityMap;
typedef	boost::property_map<Graph, boost::edge_reverse_t>::type		ReverseEdgeMap;
typedef	boost::graph_traits<Graph>::vertex_descriptor			Vertex;
typedef	boost::graph_traits<Graph>::edge_descriptor			Edge;
typedef	boost::graph_traits<Graph>::edge_iterator			EdgeIt;


// Custom Edge Adder Class, that holds the references
// to the graph, capacity map and reverse edge map
// ===================================================
class EdgeAdder {
	Graph &G;
	EdgeCapacityMap	&capacitymap;
	ReverseEdgeMap	&revedgemap;

public:
	// to initialize the Object
	EdgeAdder(Graph & G, EdgeCapacityMap &capacitymap, ReverseEdgeMap &revedgemap):
		G(G), capacitymap(capacitymap), revedgemap(revedgemap){}

	// to use the Function (add an edge)
	void addEdge(int from, int to, long capacity) {
		Edge e, rev_e;
		bool success;
		boost::tie(e, success) = boost::add_edge(from, to, G);
		boost::tie(rev_e, success) = boost::add_edge(to, from, G);
		capacitymap[e] = capacity;
		capacitymap[rev_e] = 0; // reverse edge has no capacity!
		revedgemap[e] = rev_e;
		revedgemap[rev_e] = e;
	}
};


// Functions
void testcase() {
  int n,m,a,s,c,d;
  std::cin >> n >> m >> a >> s >> c >> d;

  // Read the graph
  Graph G(n);
  WeightMap weights = get(boost::edge_weight,G);
  for(int i=0;i<m;++i){
    char w;
    int x,y,z;
    std::cin >> w >> x >> y >> z;
    Edge e; bool success;
    tie(e,success) = add_edge(x,y,G);
    weights[e] = z;
    if(w=='L'){
      tie(e,success) = add_edge(y,x,G);
      weights[e] = z;
    }
  }

  // Count number of agents in each position
  std::vector<int> n_agents(n,0);
  std::vector<int> n_shelts(n,0);
  for(int i=0;i<a;++i){
    int ag;
    std::cin >> ag;
    ++n_agents[ag];
  }
  // Count number of shelters at each position
  for(int i=0;i<s;++i){
    int sh;
    std::cin >> sh;
    ++n_shelts[sh];
  }
  // [ <start point, number of agents> ]
  std::vector<std::pair<int,int>> agents;
  std::vector<std::pair<int,int>> shelts;
  for(int i=0;i<n;++i){
    if(n_agents[i] !=0) agents.push_back(std::make_pair(i,n_agents[i]));
    if(n_shelts[i] !=0) shelts.push_back(std::make_pair(i,n_shelts[i]));
  }

  // Perform dijkstra from each agent to see the shortest time to reach each shelter
  // [ <-dist, agent, shelter> ]
  std::vector<std::tuple<long,int,int>> dists;
  long y = 1<<30;
  for(auto ag : agents){
    std::vector<Vertex> predmap(n);	// We will use this vector as an Exterior Property Map: Vertex -> Dijkstra Predecessor
	  std::vector<long> distmap(n,std::numeric_limits<int>::max());		// We will use this vector as an Exterior Property Map: Vertex -> Distance to source
	  Vertex start = ag.first;
	  boost::dijkstra_shortest_paths(G, start, // We MUST provide at least one of the two maps
		  boost::predecessor_map(
		    boost::make_iterator_property_map(predmap.begin(), 
		      boost::get(boost::vertex_index, G))).	// predecessor map as Named Parameter
		  distance_map(boost::make_iterator_property_map(distmap.begin(), 
		      boost::get(boost::vertex_index, G))));	// distance map as Named Parameter
		for(auto sh : shelts){
		  if(distmap[sh.first] < std::numeric_limits<int>::max()){
		    dists.push_back(std::make_tuple(distmap[sh.first],ag.first,sh.first));
		  }
		}
  }

  std::sort(dists.begin(),dists.end());

  // Binary search to find minimal time required
  long l=0;
  long r=1<<30;
  long q;
  while(l<=r){
    q = l + (r-l)/2;
    Graph G2(n*3);
    Vertex src = add_vertex(G2);
    Vertex trg = add_vertex(G2);
	  EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G2);
	  ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G2);
	  ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G2);
	  EdgeAdder eaG(G2, capacitymap, revedgemap);
    for(int i=0;i<dists.size();++i){
      long arrival = get<0>(dists[i]);
      if(arrival>q) break;
      int agent_pos = get<1>(dists[i]);
      int shelter_pos = get<2>(dists[i]);
      eaG.addEdge(agent_pos,2*n+shelter_pos,a);
      if(c==2 && arrival+d<=q)
        eaG.addEdge(agent_pos,n+shelter_pos,a);
    }
    for(auto ag : agents){
      eaG.addEdge(src,ag.first,ag.second);
    }
    for(auto sh : shelts){
      eaG.addEdge(n+sh.first,trg,sh.second);
      eaG.addEdge(2*n+sh.first,trg,sh.second);
    }
	  long flow = boost::edmonds_karp_max_flow(G2, src, trg);

    if(flow<a) l = q+1;
    else r = q-1;
  }
  q = l + (r-l)/2;

  std::cout << (q+d) << std::endl;
}

int main() {
	std::ios_base::sync_with_stdio(false);
	int T;
  std::cin >> T;
  while(T--) testcase();
	return 0;
}

