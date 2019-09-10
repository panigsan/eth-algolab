// ALGOLAB BGL Tutorial 1
// Tutorial example problem

// Compile and run with one of the following:
// g++ -std=c++11 -O2 bgl-tutorial_problem.cpp -o bgl-tutorial_problem; ./bgl-tutorial_problem < bgl-tutorial_problem.in
// g++ -std=c++11 -O2 -I path/to/boost_1_58_0 bgl-tutorial_problem.cpp -o bgl-tutorial_problem; ./bgl-tutorial_problem < bgl-tutorial_problem.in

// Includes
// ========
// STL includes
#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/strong_components.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
// Namespaces
using namespace std;
using namespace boost;

#define trace(x) cerr << #x << " = " << x << endl


// Directed graph with integer weights on edges.
typedef adjacency_list<vecS, vecS, directedS,
		no_property,
		property<edge_weight_t, long>
		>		Graph;
typedef graph_traits<Graph>::vertex_descriptor		Vertex;	// Vertex type		
typedef graph_traits<Graph>::edge_descriptor		Edge;	// Edge type
typedef graph_traits<Graph>::edge_iterator		EdgeIt;	// Edge iterator
// Property map edge -> weight
typedef property_map<Graph, edge_weight_t>::type	WeightMap;


// Functions
// ========= 
void testcases() {
  int k;
  cin >> k;
	// Read and build graph
	Graph G(k*k);	// Creates an empty graph on V vertices
	WeightMap weightmap = get(edge_weight, G);
  vector<vector<long>> W(k);
  for(int i=0;i<k;++i){
    for(int j=0;j<=i;++j){
      int w;
      cin >> w;
      W[i].push_back(w);
    }
  }
  for(int i=0;i<k;++i){
    for(int j=0;j<=i;++j){
      Edge e; bool success;
      // add edges going from left to right
      if(j<i){
        tie(e,success) = add_edge(i*k+j,i*k+j+1,G);
        weightmap[e] = W[i][j+1];
        tie(e,success) = add_edge(i*k+j+1,i*k+j,G);
        weightmap[e] = W[i][j];
      }
      // add edges going from top to bottom
      if(i<k-1){
        tie(e,success) = add_edge(i*k+j,(i+1)*k+j,G);
        weightmap[e] = W[i+1][j];
        tie(e,success) = add_edge((i+1)*k+j,i*k+j,G);
        weightmap[e] = W[i][j];
        tie(e,success) = add_edge(i*k+j,(i+1)*k+j+1,G);
        weightmap[e] = W[i+1][j+1];
        tie(e,success) = add_edge((i+1)*k+j+1,i*k+j,G);
        weightmap[e] = W[i][j];
      }
    }
  }

  vector<vector<long>> distmap(3,vector<long>(k*k));
  vector<vector<Vertex>> predmap(3,vector<Vertex>(k*k));
  vector<int> starts({0,(k-1)*k,k*k-1});
  for(int i=0;i<3;++i){
    dijkstra_shortest_paths(G, starts[i],
    		predecessor_map(make_iterator_property_map(	// named parameters
    				predmap[i].begin(), get(vertex_index, G))).
    		distance_map(make_iterator_property_map(	// concatenated by .
    				distmap[i].begin(), get(vertex_index, G))));
  }

  long best=numeric_limits<long>::max();
  for(int i=0;i<k;++i){
    for(int j=0;j<=i;++j){
      // sum the distances between the three points and remove twice the current
      // weight (since it has been countet three times)
      long current = distmap[0][i*k+j] + distmap[1][i*k+j]+distmap[2][i*k+j]-2*W[i][j];
      best = min(best,current);
    }
  }

  cout << best << endl;
}

int main() {
	ios_base::sync_with_stdio(false);
	int T;	cin >> T;	// First input line: Number of testcases.
	while(T--)	testcases();
	return 0;
}
