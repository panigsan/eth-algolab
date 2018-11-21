#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
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

using namespace boost;
typedef std::size_t I;
typedef adjacency_list < vecS, vecS, undirectedS,
  no_property, property < edge_weight_t, I > > Graph;
typedef graph_traits < Graph >::edge_descriptor Edge;
typedef graph_traits < Graph >::out_edge_iterator OutEdgeIt;
typedef graph_traits < Graph >::vertex_descriptor Vertex;

const I INF = std::numeric_limits<I>::max();

void testcase()
{
  // read number of points
  int n,m;
  I p;
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

  // Spanning tree on triangulation
  Graph G1(n);
  property_map < Graph, edge_weight_t >::type weight1 = get(edge_weight, G1);
  for(auto v = t.finite_vertices_begin();v!=t.finite_vertices_end();++v){
    auto c = t.incident_vertices(v);
    if(c==0) continue;
    do{
      if(t.is_infinite(c)) continue;
      Edge e; bool success;
      tie(e,success) = add_edge(v->info(),c->info(),G1);
      weight1[e] = CGAL::squared_distance(v->point(),c->point());
    } while (++c != t.incident_vertices(v));
  }

  std::vector < Edge > spanning_tree;
  kruskal_minimum_spanning_tree(G1, std::back_inserter(spanning_tree));

  Graph G2(n);
  property_map < Graph, edge_weight_t >::type weight2 = get(edge_weight, G2);
  for(Edge e_ : spanning_tree){
    Edge e; bool success;
    tie(e,success) = add_edge(source(e_,G1),target(e_,G1),G2);
    weight2[e] = weight1[e_];
  }

  I a = 0;
  I b = 0;
  for(int i=0;i<m;++i){
    K::Point_2 p0,p1;
    std::cin >> p0 >> p1;

    Vertex_handle v0 = t.nearest_vertex(p0);
    Vertex_handle v1 = t.nearest_vertex(p1);
    I g2 = std::max(CGAL::squared_distance(p0,v0->point()),
                    CGAL::squared_distance(p1,v1->point()));

    int start = v0->info();
    int end = v1->info();

    std::vector<bool> visited(n,false);
    std::vector<I>  best(n,INF);
    std::vector<Vertex>  pred(n,-1);
    best[v1->info()] = 0;
    std::stack<int> Q;
    Q.push(start);
    bool keep_looking = true;
    while(!Q.empty()){
      Vertex v = Q.top();
      if(visited[v]){
        I b = best[v];
        OutEdgeIt ebeg,eend;
        for(tie(ebeg,eend) = out_edges(v,G2); ebeg!=eend;++ebeg){
          Vertex u = target(*ebeg,G2);
          if(pred[v]==u) continue;
          I bi = std::max(best[u],weight2[*ebeg]);
          if(bi < b) b = bi;
        }

        best[v] = b;
        Q.pop();
      } else {
        visited[v] = true;
        OutEdgeIt ebeg,eend;
        for(tie(ebeg,eend) = out_edges(v,G2); ebeg!=eend;++ebeg){
          Vertex u = target(*ebeg,G2);
          if(!visited[u]) {
            Q.push(u);
            pred[u] = v;
          }
        }
      }
    }

    g2 = std::max(4*g2,best[v0->info()]);
    a = std::max(a,g2);
    if(g2 <= p){
      b = std::max(b,g2);
      std::cout << 'y';
    } else {
      std::cout << 'n';
    }
  }
  std::cout << std::endl;
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
