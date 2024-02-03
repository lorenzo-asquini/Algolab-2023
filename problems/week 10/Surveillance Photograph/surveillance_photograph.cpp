#include <iostream>
#include <climits>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor>>>> graph;

typedef traits::vertex_descriptor vertex_desc;
typedef traits::edge_descriptor edge_desc;

class edge_adder {
  graph &G;

  public:
    explicit edge_adder(graph &G) : G(G) {}

    void add_edge(int from, int to, long capacity) {
      auto c_map = boost::get(boost::edge_capacity, G);
      auto r_map = boost::get(boost::edge_reverse, G);
      const auto e = boost::add_edge(from, to, G).first;
      const auto rev_e = boost::add_edge(to, from, G).first;
      c_map[e] = capacity;
      c_map[rev_e] = 0;
      r_map[e] = rev_e;
      r_map[rev_e] = e;
    }
};

int main(){
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin>>t;
  while(t--){
    int n, m, k, l;  //n = intersections, m = streets, k = p stations, l = photos
    std::cin>>n>>m>>k>>l;

    graph G(2*n);
    edge_adder adder(G);

    const vertex_desc source = boost::add_vertex(G);
    const vertex_desc sink = boost::add_vertex(G);

    //Divide each vertex into two

    //Connect one part of each station to the source and one to the sink
    //From the source a policeman can go out, and return with a photo to the sink
    for(int i = 0; i < k; i++){
      int x;
      std::cin>>x;

      adder.add_edge(source, 2*x, 1);
      adder.add_edge(2*x+1, sink, 1);
    }

    //Use each photo intersection once for each photo
    //This considers also the special case when there are photos in the same intersection
    //As a police station (immediately taken if possible)
    for(int i = 0; i < l; i++){
      int x;
      std::cin>>x;

      adder.add_edge(2*x, 2*x+1, 1);
    }

    for(int i = 0; i < m; i++){
      int x, y;
      std::cin>>x>>y;

      //Either go through the even path that consider the roads all clear
      //Or go through the odd path  where photos are collected and roads can be used only once
      adder.add_edge(2*x, 2*y, INT_MAX);
      adder.add_edge(2*x+1, 2*y+1, 1);
    }

    long flow = boost::push_relabel_max_flow(G, source, sink);
    std::cout << flow << "\n";
  }
}
