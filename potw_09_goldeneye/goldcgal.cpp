#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Union_find.h>
#include <iostream>
#include <vector>

#define trace(x) std::cerr << #x << " = " << x << std::endl

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_with_info_2<int,K> Vb;
typedef CGAL::Triangulation_face_base_2<K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>  Triangulation;
typedef Triangulation::Vertex_handle Vertex_handle;
typedef std::size_t ll;

const ll INF = std::numeric_limits<ll>::max();

void testcase()
{

  // read number of points
  int n,m;
  long p;
  std::cin >> n >> m >> p;
  // read points
  std::vector<K::Point_2> pts(n);
  for (int i = 0; i < n; ++i) {
    std::cin >> pts[i];
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
  typedef CGAL::Union_find<int> UF;
  UF uf;
  std::vector<UF::handle> G(n);
  for(int i=0;i<n;++i) G[i] = uf.make_set(i);
  
  int l=0;
  ll a=0;
  for(int i=0;i<m;++i){
    while(!uf.same_set(G[mis[i].first],G[mis[i].second])){
      a = std::max(a,edges[l].first);
      int u = edges[l].second.first;
      int v = edges[l].second.second;
      uf.unify_sets(G[u],G[v]);
      ++l;
    }
    a = std::max(a,mis_d[i]*4);
  }

  // compute b
  uf.clear();
  for(int i=0;i<n;++i) G[i] = uf.make_set(i);
  std::vector<bool> in(m);
  for(auto e : edges){
    if(e.first > p) break;
    int u = e.second.first;
    int v = e.second.second;
    uf.unify_sets(G[u],G[v]);
  }

  for(int i=0;i<m;++i){
    in[i] = mis_d[i]*4 <= p && uf.same_set(G[mis[i].first],G[mis[i].second]);
    std::cout << (in[i] ? 'y' : 'n');
  }
  std::cout << std::endl;


  uf.clear();
  for(int i=0;i<n;++i) G[i] = uf.make_set(i);
  l=0;
  ll b=0;
  for(int i=0;i<m;++i){
    if(!in[i]) continue;
    while(!uf.same_set(G[mis[i].first],G[mis[i].second])){
      b = std::max(b,edges[l].first);
      int u = edges[l].second.first;
      int v = edges[l].second.second;
      uf.unify_sets(G[u],G[v]);
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
