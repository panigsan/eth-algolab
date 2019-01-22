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

typedef CGAL::Triangulation_vertex_base_with_info_2<EP,EK> Vb;
typedef CGAL::Triangulation_face_base_with_info_2<Info,EK> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<EK,Tds>  Triangulation;
typedef Triangulation::Face_handle Face_handle;

const EK::FT INF(std::numeric_limits<double>::max());

void testcase(){
  int n,m;
  EK::FT r;
  std::cin >> n >> m >> r;

  // Read the trees
  std::vector<EP> trees(n);
  for(int i=0;i<n;++i){
    long x,y;
    std::cin >> x >> y;
    trees[i] = EP(x,y);
  }

  Triangulation t;
  t.insert(trees.begin(),trees.end());

  for(int i=0;i<m;++i){
    long x,y;
    EK::FT s;
    std::cin >> x >> y >> s;
    EP center(x,y);

    EP nearest = t.nearest_vertex(center)->point();
    EK::FT d = CGAL::squared_distance(center,nearest);
    EK::FT required = 4*CGAL::square(r+s);
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

