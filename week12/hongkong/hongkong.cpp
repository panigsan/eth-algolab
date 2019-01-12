#include <iostream>
#include <vector>
#include <queue>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>

#define trace(x) std::cerr << #x << " = " << x << std::endl

typedef CGAL::Exact_predicates_exact_constructions_kernel EK;
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
// Info of a face
struct Info{
  long id;
  K::FT dist;    // squared distance from center of triangle to nearest vertex
  K::FT max_out; // max radius that a balloon was able to get out
};

typedef CGAL::Triangulation_vertex_base_2<K> Vb;
typedef CGAL::Triangulation_face_base_with_info_2<Info,K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>  Triangulation;
typedef Triangulation::Finite_faces_iterator  Face_iterator;
typedef K::Point_2 P;

void testcase(){
  int n,m;
  K::FT r;
  std::cin >> n >> m >> r;

  std::vector<P> trees;
  for(int i=0;i<n;++i){
    K::FT x,y;
    std::cin >> x >> y;
    trees.push_back(P(x,y));
  }

  Triangulation t;
  t.insert(trees.begin(),trees.end());
  
  // assign an id to all faces and default values
  int id=0;
  for(auto it=t.all_faces_begin();it!=t.all_faces_end();++it){
    it->info().id=id++;
    it->info().dist=-1;
    it->info().max_out=-1;
  }

  // vector of <s, point, id> of the balloons
  std::vector<std::tuple<K::FT,P,int>> ballons;
  for(int i=0;i<m;++i){
    K::FT x,y,s;
    std::cin >> x >> y >> s;
    P start(x,y);
    ballons.push_back(std::make_tuple(-s,start,i));
  }
  // sort the balloons so that it computes larger balloons first
  std::sort(ballons.begin(),ballons.end());

  // vector to store the solutions
  std::vector<bool> sols(m);
  for(int i=0;i<m;++i){
    int j = get<2>(ballons[i]);
    K::FT s = -get<0>(ballons[i]);
    P start = get<1>(ballons[i]);

    K::FT req_space = CGAL::square(2.0*(r+s));
    // if it cannot be inflated
    auto vh = t.nearest_vertex(start);
    if(CGAL::squared_distance(vh->point(),start)<CGAL::square(r+s)){
      sols[j] = false;
      continue;
    }

    std::priority_queue<std::pair<K::FT,Triangulation::Face_handle>> Q;
    bool found=false;
    std::vector<bool> visited(id,false);

    auto fh1 = t.locate(start);
    // start from current face
    // and perform a "prioritised" search
    Q.push(std::make_pair(-fh1->info().dist,fh1));
    while(!Q.empty()){
      auto fh = Q.top().second; Q.pop();
      if(visited[fh->info().id]) continue;

      visited[fh->info().id]=true;
      if(t.is_infinite(fh)){
        found = true;
        break;
      }
      // if a balloon with greater size was able to get out from here, the 
      // current balloon can get out too
      if(fh->info().max_out>=req_space){
        found=true;
        break;
      }
      // compute the distance from the center of the triangle to the nearest vertex
      // if it has not been computed yet
      if(fh->info().dist==-1){
        K::Circle_2 cir(fh->vertex(0)->point(),
                        fh->vertex(1)->point(),
                        fh->vertex(2)->point());
        fh->info().dist = cir.squared_radius();
        /*
        P dual = t.dual(fh);
        auto vh = fh->vertex(0);//t.nearest_vertex(dual,fh);
        fh->info().dist = CGAL::squared_distance(dual,vh->point());
        */
      }
      // if it can get out, updated internal values for speedup
      if(fh->info().dist>=req_space){
        found=true;
        fh->info().max_out = std::max(fh->info().max_out,req_space);
        fh1->info().max_out = std::max(fh1->info().max_out,fh->info().max_out);
        break;
      }

      // add a neighbor face if the ballon can get through
      if(CGAL::squared_distance(fh->vertex(0)->point(),fh->vertex(1)->point())>=req_space)
        Q.push(std::make_pair(-fh->neighbor(2)->info().dist,fh->neighbor(2)));
      if(CGAL::squared_distance(fh->vertex(1)->point(),fh->vertex(2)->point())>=req_space)
        Q.push(std::make_pair(-fh->neighbor(0)->info().dist,fh->neighbor(0)));
      if(CGAL::squared_distance(fh->vertex(2)->point(),fh->vertex(0)->point())>=req_space)
        Q.push(std::make_pair(-fh->neighbor(1)->info().dist,fh->neighbor(1)));

    }

    sols[j] = found;
  }
  for(int i=0;i<m;++i) std::cout << (sols[i] ? 'y' : 'n');
  std::cout << std::endl;
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int T;
  std::cin >> T;
  while(T--) testcase();
}

