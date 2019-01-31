#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/connected_components.hpp>
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
                boost::no_property,
                boost::no_property > Graph;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_with_info_2<int,K> Vb;
typedef CGAL::Triangulation_face_base_2<K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>  Triangulation;

typedef K::Point_2 P;

#define trace(x) std::cerr << #x << " = " << x << std::endl

void testcase(){
    int n;
    long r2;
    std::cin >> n >> r2;
    r2 = r2*r2;

    std::vector<std::pair<P,int>> planets;
    for(int i=0;i<n;++i){
        long x,y;
        std::cin >> x >> y;
        planets.push_back({P(x,y),i});
    }

    int l=1;
    int r=n;
    int k=(l+r)/2;
    int best_k=0;
    while(l<r){
        Triangulation t;
        t.insert(planets.begin()+k,planets.end());

        Graph G(n);
        for(auto it = t.finite_edges_begin();it!=t.finite_edges_end();++it){
            Triangulation::Vertex_handle v1 = it->first->vertex((it->second + 1) % 3);
            Triangulation::Vertex_handle v2 = it->first->vertex((it->second + 2) % 3);
            if(CGAL::squared_distance(v1->point(),v2->point())<=r2){
                boost::add_edge(v1->info(),v2->info(),G);    
            }
        }
        std::vector<int> componentmap(n);       // We MUST use such a vector as an Exterior Property Map: Vertex -> Component
        int ncc = boost::connected_components(G, boost::make_iterator_property_map(componentmap.begin(), boost::get(boost::vertex_index, G)));
        std::vector<int> componentsize(ncc);
        // Iterate over all vertices
        for (int i = 0; i < n; ++i)
            ++componentsize[componentmap[i]];
        int max_connected = *std::max_element(componentsize.begin(),componentsize.end());
        if(max_connected==k) break;
        if(max_connected>k){
            if(l==k) break;
            l = k;
        }
        else{
            if(r==k) break;
            r = k;
        }
        k = (l+r)/2;
    }

    std::cout << k << std::endl;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int T;
    std::cin >> T;
    while(T--) testcase();
    return 0;
}

