#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <vector>
#include <queue>
#include <algorithm>

#define trace(x) std::cerr << #x << " = " << x << std::endl

typedef CGAL::Exact_predicates_exact_constructions_kernel EK;
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;

typedef EK::Point_2 EP;
typedef K::Point_2 P;

struct Info{};

typedef CGAL::Triangulation_vertex_base_with_info_2<EP,K> Vb;
typedef CGAL::Triangulation_face_base_with_info_2<Info,K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>  Triangulation;
typedef Triangulation::Face_handle Face_handle;

const K::FT INF(std::numeric_limits<double>::max());

void testcase(){
  int n,m;
  K::FT r;
  std::cin >> n >> m >> r;

  // Read the trees
  std::vector<P> trees(n);
  for(int i=0;i<n;++i){
    K::FT x,y;
    std::cin >> x >> y;
    trees[i] = P(x,y);
  }

  Triangulation t;
  t.insert(trees.begin(),trees.end());

  // To each vertex, assign an exact point
  for(auto it=t.finite_vertices_begin(); it!=t.finite_vertices_end();++it){
    it->info() = EP(it->point().x(),it->point().y());
  }

  for(int i=0;i<m;++i){
    K::FT x,y;
    EK::FT s;
    std::cin >> x >> y >> s;
    P center(x,y);
    EP e_center(x,y);

    EP nearest = t.nearest_vertex(center)->info();
    EK::FT d = CGAL::squared_distance(e_center,nearest);
    EK::FT required = (r+s)*(r+s);
    if(d < required){
      std::cout << 'n';
    } else {
      std::cout << 'y';
    }
  }
  std::cout << std::endl;

}

int main(){
  std::ios_base::sync_with_stdio(false);
  int T;
  std::cin >> T;
  while(T--) testcase();
}

