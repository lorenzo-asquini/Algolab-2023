#include <iostream>
#include <vector>
#include <algorithm>

#include <boost/pending/disjoint_sets.hpp>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/connected_components.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> graph;

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_with_info_2<int, K>  Vb;
typedef CGAL::Triangulation_face_base_2<K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb, Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds> Triangulation;

typedef K::Point_2 P;

typedef struct{
  int from;
  int to;
  long sq_len;
} edge_t;  //Edge in the triangulation

struct orderEdges{
  bool operator()(edge_t &e1, edge_t &e2){
    return e1.sq_len < e2.sq_len;
  }
};

int main(){
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin>>t;
  while(t--){
    int n, m;  //n = jammers, m = missions
    long p;  //p = initial power
    std::cin>>n>>m>>p;

    std::vector<std::pair<P, int>> jammers(n);
    for(int i = 0; i < n; i++){
      int x, y;
      std::cin>>x>>y;

      jammers[i] = {P(x, y), i};
    }

    Triangulation tri;
    tri.insert(jammers.begin(), jammers.end());

    std::vector<edge_t> covered_edges;  //Edges covered with the inital power
    std::vector<edge_t> all_edges;  //All edges

    for (auto e = tri.finite_edges_begin(); e != tri.finite_edges_end(); e++) {
      //An edge is stored as a pair of a face and the index (from 0 to 2) of the vertex
      //which is opposite to the edge in the face. This means that:
      //face = e->first
      //v1 = (e->second+1)%3, v2 = (e->second+2)%3
      //At this point, it's possible to get the indexes of the vertices at the triangulation level
      int i1 = e->first->vertex((e->second+1)%3)->info();
      int i2 = e->first->vertex((e->second+2)%3)->info();

      long sq_len = tri.segment(e).squared_length();

      //Safe if len/2 <= sqrt(p/4) -> len^2 <= p
      if(sq_len <= p){
        covered_edges.push_back({i1, i2, sq_len});
      }
      all_edges.push_back({i1, i2, sq_len});
    }

    //Sort the edges by the squared length
    std::sort(covered_edges.begin(), covered_edges.end(), orderEdges());
    std::sort(all_edges.begin(), all_edges.end(), orderEdges());

    //Find connected components with BGL
    graph G(n);
    for(edge_t e: covered_edges){
      boost::add_edge(e.from, e.to, G);
    }
    std::vector<int> component(boost::num_vertices(G));
    connected_components(G, &component[0]);

    //Disjointed sets for faster dynamic connected components
    boost::disjoint_sets_with_storage<> all_graph(n);
    boost::disjoint_sets_with_storage<> covered_graph(n);

    auto all_it = all_edges.begin();
    auto covered_it = covered_edges.begin();

    long power_all = 0;
    long power_covered = 0;

    for(int i = 0; i < m; i++){
      int x0, y0, x1, y1;
      std::cin>>x0>>y0>>x1>>y1;

      P start(x0, y0);
      P end(x1, y1);

      //Find the distance to the nearest jammers to the start position of the mission
      auto near_start = tri.nearest_vertex(start);
      int near_start_index = near_start->info();
      long sq_d_start = CGAL::squared_distance(start, near_start->point());

      //Find the distance to the nearest jammers to the end position of the mission
      auto near_end = tri.nearest_vertex(end);
      int near_end_index = near_end->info();
      long sq_d_end = CGAL::squared_distance(end, near_end->point());

      //Update the power to execute all mission making sure to cover both start and end positions
      power_all = std::max(power_all, sq_d_start*4);
      power_all = std::max(power_all, sq_d_end*4);

      //Add edges to the disjointed set until the jammer near the start and the jammer near
      //the end are both in the same connected component
      int near_start_set, near_end_set;
      while(all_it != all_edges.end() &&
        ((near_start_set = all_graph.find_set(near_start_index)) != (near_end_set = all_graph.find_set(near_end_index)))){

        power_all = std::max(power_all, all_it->sq_len);

        int from_set = all_graph.find_set(all_it->from);
        int to_set = all_graph.find_set(all_it->to);

        if(from_set != to_set){
          all_graph.link(all_it->from, all_it->to);
        }
        all_it++;
      }

      //Check if the start and end positions are covered by their closest jammers
      //d <= sqrt(p/4) -> d^2 <= p / 4
      if(sq_d_start * 4 > p){
        std::cout<<"n";
        continue;
      }

      if(sq_d_end * 4 > p){
        std::cout<<"n";
        continue;
      }

      //Check if the jammers near the start and the end positions are in the same connected components
      if(component[near_start_index] != component[near_end_index]){
        std::cout<<"n";
        continue;
      }

      //The mission can be done
      std::cout<<"y";

      //Update the power to execute the same mission making sure to cover both start and end positions
      power_covered = std::max(power_covered, sq_d_start*4);
      power_covered = std::max(power_covered, sq_d_end*4);

      //Add edges to the disjointed set until the jammer near the start and the jammer near
      //the end are both in the same connected component
      while(covered_it != covered_edges.end() &&
        ((near_start_set = covered_graph.find_set(near_start_index)) != (near_end_set = covered_graph.find_set(near_end_index)))){

        power_covered = std::max(power_covered, covered_it->sq_len);

        int from_set = covered_graph.find_set(covered_it->from);
        int to_set = covered_graph.find_set(covered_it->to);

        if(from_set != to_set){
          covered_graph.link(covered_it->from, covered_it->to);
        }
        covered_it++;
      }
    }

    std::cout<<"\n";
    std::cout<<power_all<<"\n";
    std::cout<<power_covered<<"\n";
  }
}
