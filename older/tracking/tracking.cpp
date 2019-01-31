#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <cassert>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

// BGL Graph definitions
// =====================
// Graph Type, OutEdgeList Type, VertexList Type, (un)directedS
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,             // Use vecS for the VertexList! Choosing setS for the OutEdgeList disallows parallel edges.
            boost::no_property,                             // interior properties of vertices      
            boost::property<boost::edge_weight_t, int>              // interior properties of edges
            >                                       Graph;
typedef boost::graph_traits<Graph>::edge_descriptor             Edge;           // Edge Descriptor: an object that represents a single edge.
typedef boost::graph_traits<Graph>::vertex_descriptor           Vertex;         // Vertex Descriptor: with vecS vertex list, this is really just an int in the range [0, num_vertices(G)).      
typedef boost::graph_traits<Graph>::edge_iterator               EdgeIt;         // to iterate over all edges
typedef boost::graph_traits<Graph>::out_edge_iterator           OutEdgeIt;      // to iterate over all outgoing edges of a vertex
typedef boost::property_map<Graph, boost::edge_weight_t>::type  WeightMap;      // property map to access the interior property edge_weight_t

class EdgeAdder {
        Graph &G;
        WeightMap &weightMap;

public:
        // to initialize the Object
        EdgeAdder(Graph & G, WeightMap &weightMap):
                G(G), weightMap(weightMap){}

        // to use the Function (add an edge)
        void addEdge(int from, int to, long capacity) {
                Edge e, rev_e;
                bool success;
                boost::tie(e, success) = boost::add_edge(from, to, G);
                weightMap[e]=capacity;
        }
};



void testcase(){
    int n,m,k,x,y;
    std::cin >> n >> m >> k >> x >> y;

    Graph G(n*(k+1));
    WeightMap weightmap = boost::get(boost::edge_weight, G);
    EdgeAdder eaG(G,weightmap);

    for(int i=0;i<m;++i){
        int a,b,c,d;
        std::cin >> a >> b >> c >> d;

        for(int j=0;j<=k;++j){
            if(d==0) eaG.addEdge(n*j+a,n*j+b,c);
            else {
                // if along river then goes to the next level 
                // or stays in the same if the last level has
                // been reached
                eaG.addEdge(n*j+a,std::min(n*k+b,n*(j+1)+b),c);
                eaG.addEdge(n*j+b,std::min(n*k+a,n*(j+1)+a),c);
            }
        }
    }
    std::vector<Vertex> predmap(n*(k+1));     // We will use this vector as an Exterior Property Map: Vertex -> Dijkstra Predecessor
    std::vector<int> distmap(n*(k+1));            // We will use this vector as an Exterior Property Map: Vertex -> Distance to source
    boost::dijkstra_shortest_paths(G, x, // We MUST provide at least one of the two maps
    boost::predecessor_map(boost::make_iterator_property_map(predmap.begin(), boost::get(boost::vertex_index, G))). // predecessor map as Named Parameter
    distance_map(boost::make_iterator_property_map(distmap.begin(), boost::get(boost::vertex_index, G))));  // distance map as Named Parameter

    std::cout << distmap[n*k+y] << std::endl;
    
}

int main(){
    std::ios_base::sync_with_stdio(false);
    int T;
    std::cin >> T;
    while(T--) testcase();
}

