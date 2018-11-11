#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>
#include <iostream>
#include <utility>
#include <vector>
#include <queue>
#include <deque>

#define step(x) std::cerr << "Step " << x << std::endl
#define trace(x) std::cerr << #x << " = " << x << std::endl

enum Color { White = 0, Visited = 1, Infinite = 2 };
struct MyInfo {
  Color color;
  long d01;
  long d12;
  long d20;
} ;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_2<K> Vb;
typedef CGAL::Triangulation_face_base_with_info_2<MyInfo,K> Fb;
//typedef CGAL::Triangulation_face_base_with_info_2<std::pair<Color,K::Point_2>,K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>  Triangulation;
typedef Triangulation::Finite_faces_iterator  Face_iterator;

int main()
{
  std::ios_base::sync_with_stdio(false);

  while(true){
    // read number of points
    std::size_t n;
    std::cin >> n;
    if(n==0) break;

    // construct triangulation
    Triangulation t;
    step("read");
    for (std::size_t i = 0; i < n; ++i) {
      Triangulation::Point p;
      std::cin >> p;
      t.insert(p);
    }
    step("go");

    for(auto f = t.all_faces_begin(); f!= t.all_faces_end(); ++f){
      f->info().d01 = CGAL::squared_distance(f->vertex(0)->point(),f->vertex(1)->point());
      f->info().d12 = CGAL::squared_distance(f->vertex(1)->point(),f->vertex(2)->point());
      f->info().d20 = CGAL::squared_distance(f->vertex(2)->point(),f->vertex(0)->point());
      if(t.is_infinite(f)){
        f->info().color = Infinite;
      } else {
        f->info().color = White;
      }
    }
    step(2);

    int m;
    std::cin >> m;
    for(int i=0;i<m;++i){
      K::Point_2 p;
      long d;
      std::cin >> p >> d;

      Triangulation::Face_handle f2 = t.locate(p);

      if(CGAL::squared_distance(p,t.nearest_vertex(p)->point())<d) {
        std::cout << 'n';
        continue;
      }
      for(Face_iterator f3 = t.finite_faces_begin(); f3 != t.finite_faces_end(); ++f3){
        f3->info().color = White;
      }

      bool found=false;
      std::deque<Triangulation::Face_handle> Q;
      Q.push_front(f2);
      while(!Q.empty()){
        f2 = Q.front(); Q.pop_front();
        if(f2->info().color == Visited) continue;
        if(f2->info().color == Infinite) {
          found=true;
          break;
        }


        if(f2->info().d01 >= 4*d) Q.push_front(f2->neighbor(2));
        if(f2->info().d12 >= 4*d) Q.push_front(f2->neighbor(0));
        if(f2->info().d20 >= 4*d) Q.push_front(f2->neighbor(1));
        /*
        for(int j=0;j<3;++j){
          if(CGAL::squared_distance(f2->vertex(j)->point(),f2->vertex((j+1)%3)->point())>=4*d){
            Q.push_front(f2->neighbor((j+2)%3));    
          }
        }
        */
        f2->info().color = Visited;
      }

      std::cout << (found ? 'y' : 'n');
    }
    std::cout << std::endl;
  }
}
