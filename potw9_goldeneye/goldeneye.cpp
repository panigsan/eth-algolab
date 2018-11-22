#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/connected_components.hpp>
#include <boost/graph/incremental_components.hpp>
#include <boost/pending/disjoint_sets.hpp>
#include <iostream>
#include <queue>
#include <vector>
#include <map>

#define trace(x) std::cerr << #x << " = " << x << std::endl

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_with_info_2<int,K> Vb;
typedef CGAL::Triangulation_face_base_2<K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>  Triangulation;
typedef Triangulation::Vertex_handle Vertex_handle;
typedef std::size_t ll;

using namespace boost;
typedef adjacency_list <vecS, vecS, undirectedS> Graph;
typedef graph_traits<Graph>::vertex_descriptor Vertex;
typedef graph_traits<Graph>::vertices_size_type VertexIndex;
typedef VertexIndex* Rank;
typedef Vertex* Parent;

/*
typedef adjacency_list < vecS, vecS, undirectedS,
  no_property, no_property > Graph;
typedef graph_traits < Graph >::edge_descriptor Edge;
typedef graph_traits < Graph >::edge_iterator EdgeIt;
typedef graph_traits < Graph >::vertex_descriptor Vertex;
*/

const ll INF = std::numeric_limits<ll>::max();

void testcase()
{
  // read number of points
  int n,m;
  long p;
  std::cin >> n >> m >> p;
  // read points
  std::vector<K::Point_2> pts;
  pts.reserve(n);
  for (int i = 0; i < n; ++i) {
    K::Point_2 pnt;
    std::cin >> pnt;
    pts.push_back(pnt);
  }
  // construct triangulation
  Triangulation t;
  t.insert(pts.begin(), pts.end());

  int i=0;
  for(auto v = t.finite_vertices_begin();v!=t.finite_vertices_end();++v){
    v->info() = i++;
  }

  std::vector<std::pair<int,int>> mis(m);
  std::vector<ll> mis_d(m);
  for(int i=0;i<m;++i){
    K::Point_2 p0, p1;
    std::cin >> p0 >> p1;
    Vertex_handle v0 = t.nearest_vertex(p0);
    Vertex_handle v1 = t.nearest_vertex(p1);
    mis[i].first = v0->info();
    mis[i].second = v1->info();
    mis_d[i] = std::max(
          CGAL::squared_distance(p0,v0->point()),
          CGAL::squared_distance(p1,v1->point()));
  }


  // <dist, <u,v>>
  std::vector<std::pair<ll,std::pair<int,int>>> edges;

  for(Triangulation::Finite_edges_iterator it = t.finite_edges_begin();
      it != t.finite_edges_end(); ++it){
    ll dist = t.segment(*it).squared_length();
    Vertex_handle v0 = it->first->vertex((it->second + 1) % 3);
    Vertex_handle v1 = it->first->vertex((it->second + 2) % 3);
    int u = std::min(v0->info(),v1->info());
    int v = std::max(v0->info(),v1->info());
    edges.push_back(std::make_pair(dist,
                      std::make_pair(u,v)));
  }
  
  std::sort(edges.begin(),edges.end());

  // compute a
  Graph G(n);
  std::vector<VertexIndex> rank(n);
  std::vector<Vertex> parent(n);

  disjoint_sets<Rank, Parent> ds(&rank[0], &parent[0]);
  initialize_incremental_components(G, ds);

  int l=0;
  ll a=0;
  for(int i=0;i<m;++i){
    while(ds.find_set(mis[i].first) != ds.find_set(mis[i].second)){
      a = std::max(a,edges[l].first);
      int u = edges[l].second.first;
      int v = edges[l].second.second;
      add_edge(u,v,G);
      ds.union_set(u,v);
      ++l;
    }
    a = std::max(a,mis_d[i]*4);
  }

  // compute b
  G.clear();
  G = Graph(n);
  std::vector<bool> in(m);
  rank.clear();
  rank.resize(n);
  parent.clear();
  parent.resize(n);
  ds = disjoint_sets<Rank,Parent>(&rank[0],&parent[0]);
  initialize_incremental_components(G, ds);
  for(auto e : edges){
    if(e.first > p) break;
    int u = e.second.first;
    int v = e.second.second;
    add_edge(u,v,G);
    ds.union_set(u,v);
  }

  for(int i=0;i<m;++i){
    in[i] = ds.find_set(mis[i].first) == ds.find_set(mis[i].second) &&
            mis_d[i]*4<=p;
    std::cout << (in[i] ? 'y' : 'n');
  }
  std::cout << std::endl;

  G.clear();
  G = Graph(n);
  rank.clear();
  rank.resize(n);
  parent.clear();
  parent.resize(n);

  ds = disjoint_sets<Rank, Parent>(&rank[0], &parent[0]);
  initialize_incremental_components(G, ds);

  l=0;
  ll b=0;
  for(int i=0;i<m;++i){
    if(!in[i]) continue;
    while(ds.find_set(mis[i].first) != ds.find_set(mis[i].second)){
      b = std::max(b,edges[l].first);
      int u = edges[l].second.first;
      int v = edges[l].second.second;
      add_edge(u,v,G);
      ds.union_set(u,v);
      ++l;
    }
    b = std::max(b,mis_d[i]*4);
  }

  std::cout << a << std::endl;
  std::cout << b << std::endl;


  

  }

int main(){
  std::ios_base::sync_with_stdio(false);
  int T;
  std::cin >> T;
  while(T--) testcase();
  return 0;
}
