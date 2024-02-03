#include <iostream>
#include <vector>
#include <queue>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_2.h>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/connected_components.hpp>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;

typedef CGAL::Triangulation_vertex_base_with_info_2<int, K> Vb;
typedef CGAL::Triangulation_face_base_2<K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb, Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds> Triangulation;

typedef K::Point_2 P;

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> graph;
typedef boost::graph_traits<graph>::out_edge_iterator out_edge_it;

typedef struct{
  int from;
  int to;
} edge_t;  //Edge in the triangulation

int main(){
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin>>t;
  while(t--){
    long n, m, r;  //n = stations, m = clues, r = range
    std::cin>>n>>m>>r;

    std::vector<std::pair<P, int>> stations(n);  //Store stations with an index
    for(int i = 0; i < n; i++){
      int x, y;
      std::cin>>x>>y;

      stations[i] = {P(x, y), i};
    }

    Triangulation tri;
    tri.insert(stations.begin(), stations.end());

    //Find all stations that can communicate
    std::vector<edge_t> near_stations;
    for (auto e = tri.finite_edges_begin(); e != tri.finite_edges_end(); ++e){
      int i1 = e->first->vertex((e->second+1)%3)->info();
      int i2 = e->first->vertex((e->second+2)%3)->info();

      //l <= r, l^2 <= r^2
      long sq_l = tri.segment(e).squared_length();
      if(sq_l <= r*r){
        near_stations.push_back({i1, i2}); //Connect only stations that can communicate
      }
    }

    graph G(n);
    for(auto e: near_stations){
      boost::add_edge(e.from, e.to, G);
    }

    //Use BFS to know if the network is valid
    //0 = not traversed
    //1 = frequency 1
    //2 = frequency 2
    std::vector<int> frequency(n, 0);
    bool interference = false;

    for(int i = 0; i < n; i++){

      //If the node was not already traversed
      if(frequency[i] != 0){
        continue;
      }

      //If an interference was not already found
      if(interference){
        break;
      }

      std::queue<int> q;

      frequency[i] = 1;
      q.push(i);

      while(!q.empty()){
        int current = q.front(); q.pop();

        out_edge_it oe_beg, oe_end;
        for (boost::tie(oe_beg, oe_end) = boost::out_edges(current, G); oe_beg != oe_end; oe_beg++){
          int other = boost::target(*oe_beg, G);

          if(frequency[other] == 0){
            //If other node was never traversed
            //Assign the other frequency
            frequency[other] = (frequency[current] == 1) ? 2 : 1;
            q.push(other);

          }else if(frequency[other] == frequency[current]){
            interference = true;

            //Empty the queue
            while(!q.empty()){
              q.pop();
            }

            break;
          }
        }
      }
    }

    if(!interference){
      //Store all the points with one frequency or the other
      std::vector<P> freq1;
      std::vector<P> freq2;

      for(int i = 0; i < n; i++){
        if(frequency[i] == 1){
          freq1.push_back(stations[i].first);
        }else{
          freq2.push_back(stations[i].first);
        }
      }

      //Although in the original triangulation there may be no interference
      //There may be the nodes n1--n2--n3, such that the coloring prevents interference
      //Between n1 and n2, and between n2 and n3

      //However, if the distance between n1 and n3 is less than r, they may interfere even
      //if in the coloring is acceptable in the original triangulation

      //Create triangulations of nodes colored in the same way and see if there is interference
      //when passing over other nodes

      Triangulation tri1;
      tri1.insert(freq1.begin(), freq1.end());

      for (auto e = tri1.finite_edges_begin(); e != tri1.finite_edges_end(); e++){

        //l <= r, l^2 <= r^2
        long sq_l = tri1.segment(e).squared_length();
        if(sq_l <= r*r){
          interference = true;
        }
      }

      Triangulation tri2;
      tri2.insert(freq2.begin(), freq2.end());

      for (auto e = tri2.finite_edges_begin(); e != tri2.finite_edges_end(); e++){

        //l <= r, l^2 <= r^2
        long sq_l = tri2.segment(e).squared_length();
        if(sq_l <= r*r){
          interference = true;
        }
      }
    }

    std::vector<int> component_map(n);
    boost::connected_components(G,
      boost::make_iterator_property_map(component_map.begin(),
        boost::get(boost::vertex_index, G)));

    for(int i = 0; i < m; i++){
      int x1, y1, x2, y2;
      std::cin>>x1>>y1>>x2>>y2;

      P a = P(x1, y1);
      P b = P(x2, y2);

      if(interference){
        std::cout<<"n";
        continue;
      }

      long sq_d_direct = CGAL::squared_distance(a, b);

      if(sq_d_direct <= r*r){  //Try to see if a direct communication can exist
        std::cout<<"y";
        continue;
      }

      auto v_a = tri.nearest_vertex(a);
      long sq_d_a = CGAL::squared_distance(a, v_a->point());

      auto v_b = tri.nearest_vertex(b);
      long sq_d_b = CGAL::squared_distance(b, v_b->point());

      //Too far from the network
      if(sq_d_a > r*r || sq_d_b > r*r){
        std::cout<<"n";
        continue;
      }

      //If they are in the same valid connected component
      if(component_map[v_a -> info()] == component_map[v_b -> info()]){
        std::cout<<"y";
      }else{
        std::cout<<"n";
      }
    }

    std::cout<<"\n";
  }
}
