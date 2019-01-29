// ALGOLAB BGL Tutorial 2
// Flow example demonstrating
// - interior graph properties for flow algorithms
// - custom edge adder

// Compile and run with one of the following:
// g++ -std=c++11 -O2 flows.cpp -o flows ./flows
// g++ -std=c++11 -O2 -I path/to/boost_1_58_0 flows.cpp -o flows; ./flows

// Includes
// ========
// STL includes
#include <iostream>
#include <vector>
#include <algorithm>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/edmonds_karp_max_flow.hpp>
// Namespaces
// using namespace std;
// using namespace boost;


// BGL Graph definitions
// =====================
// Graph Type with nested interior edge properties for Flow Algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> Traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
        boost::property<boost::edge_capacity_t, long,
                boost::property<boost::edge_residual_capacity_t, long,
                        boost::property<boost::edge_reverse_t, Traits::edge_descriptor> > > >   Graph;
// Interior Property Maps
typedef boost::property_map<Graph, boost::edge_capacity_t>::type                EdgeCapacityMap;
typedef boost::property_map<Graph, boost::edge_residual_capacity_t>::type       ResidualCapacityMap;
typedef boost::property_map<Graph, boost::edge_reverse_t>::type         ReverseEdgeMap;
typedef boost::graph_traits<Graph>::vertex_descriptor                   Vertex;
typedef boost::graph_traits<Graph>::edge_descriptor                     Edge;
typedef boost::graph_traits<Graph>::edge_iterator                       EdgeIt;


// Custom Edge Adder Class, that holds the references
// to the graph, capacity map and reverse edge map
// ===================================================
class EdgeAdder {
        Graph &G;
        EdgeCapacityMap &capacitymap;
        ReverseEdgeMap  &revedgemap;

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

int m,n,k,c;
int in_vertex(int row, int column){
    return row*m+column;    
}

int out_vertex(int row, int column){
    return n*m + in_vertex(row,column);    
}

void testcase() {
    std::cin >> m >> n >> k >> c;
    // Create Graph and Maps
    Graph G(2*n*m);
    EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
    ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
    ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
    EdgeAdder eaG(G, capacitymap, revedgemap);

    Vertex source = boost::add_vertex(G);
    Vertex target = boost::add_vertex(G);

    for(int i=0;i<n;++i){
        for(int j=0;j<m;++j){
            eaG.addEdge(in_vertex(i,j),out_vertex(i,j),c);
            if(i>0)   eaG.addEdge(out_vertex(i,j),in_vertex(i-1,j),1);
            else      eaG.addEdge(out_vertex(i,j),target,1);
            if(i<n-1) eaG.addEdge(out_vertex(i,j),in_vertex(i+1,j),1);
            else      eaG.addEdge(out_vertex(i,j),target,1);

            if(j>0)   eaG.addEdge(out_vertex(i,j),in_vertex(i,j-1),1);
            else      eaG.addEdge(out_vertex(i,j),target,1);
            if(j<m-1) eaG.addEdge(out_vertex(i,j),in_vertex(i,j+1),1);
            else      eaG.addEdge(out_vertex(i,j),target,1);
        }    
    }

    for(int i=0;i<k;++i){
        int x,y;
        std::cin >> x >> y;
        eaG.addEdge(source,in_vertex(y,x),1);
    }

    long flow = boost::push_relabel_max_flow(G, source, target);
    std::cout << flow << std::endl;

}

// Main function to loop over the testcases
int main() {
    std::ios_base::sync_with_stdio(false);
    int T;
    std::cin >> T;
    while(T--) testcase();
    return 0;
}
