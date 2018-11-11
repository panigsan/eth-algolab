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
  long max_w;
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
    for (std::size_t i = 0; i < n; ++i) {
      Triangulation::Point p;
      std::cin >> p;
      t.insert(p);
    }

    std::priority_queue<std::pair<long,Triangulation::Face_handle>> Q;
    for(auto f = t.all_faces_begin(); f!= t.all_faces_end(); ++f){
      if(t.is_infinite(f)){
        Triangulation::Vertex_handle v1;
        Triangulation::Vertex_handle v2;
        if(!t.is_infinite(f,0)){ v1 = f->vertex(1); v2 = f->vertex(2);}
        if(!t.is_infinite(f,1)){ v1 = f->vertex(2); v2 = f->vertex(0);}
        if(!t.is_infinite(f,2)){ v1 = f->vertex(0); v2 = f->vertex(1);}

        f->info().color = Infinite;
        long d = CGAL::squared_distance(v1->point(),v2->point());
        Q.push(std::make_pair(d,f));
        f->info().max_w = d;
      } else {
        f->info().color = White;
        f->info().max_w = 0;
      }
    }

    while(!Q.empty()){
      std::pair<long,Triangulation::Face_handle> p = Q.top(); Q.pop();
      if(p.first != p.second->info().max_w) continue;
      for(int i=0;i<3;++i){
        Triangulation::Face_handle fh = p.second->neighbor(i);
        if(!t.is_infinite(fh)){
          long w = CGAL::squared_distance(fh->vertex((i+1)%3)->point(),fh->vertex((i+2)%3)->point());
          w = std::min(w,p.second->info().max_w);
          //auto dual = t.dual(fh);
          //w = std::min(w,(long)CGAL::squared_distance(dual,t.nearest_vertex(dual)->point()));
          if(w > fh->info().max_w){
            fh->info().max_w = w;
            Q.push(std::make_pair(w,fh));
          }
        }
      }
    }

    /*
    for(auto f = t.finite_faces_begin(); f != t.finite_faces_end();++f){
      trace(t.triangle(f));
      trace(f->info().max_w);
    }
    */

    int m;
    std::cin >> m;
    for(int i=0;i<m;++i){
      K::Point_2 p;
      long d;
      std::cin >> p >> d;
      
      Triangulation::Locate_type lt;
      int x;
      Triangulation::Face_handle f = t.locate(p,lt,x);
      if(lt==Triangulation::Locate_type::EDGE){
        if(f->neighbor(x)->info().max_w > f->info().max_w){
          f = f->neighbor(x);
        } 
      }
      /*
      trace(i);
      if(!t.is_infinite(f))
        trace(t.triangle(f));
      trace(f->info().max_w);
      */
      if(CGAL::squared_distance(p,t.nearest_vertex(p)->point())<d){
        std::cout << 'n';
      }
      else if(t.is_infinite(f)){
        std::cout << 'y';
      }
      else if(f->info().max_w > 4*d){
        std::cout << 'y';
      } else {
        std::cout << 'n';
      }
    }
    std::cout << std::endl;


    /*
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
        f2->info().color = Visited;
      }

      std::cout << (found ? 'y' : 'n');
    }
    std::cout << std::endl;
    */
  }
}
