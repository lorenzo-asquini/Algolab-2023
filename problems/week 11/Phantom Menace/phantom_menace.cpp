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
      c_map[rev_e] = 0; // reverse edge has no capacity!
      r_map[e] = rev_e;
      r_map[rev_e] = e;
    }
};

int main(){
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin>>t;
  while(t--){
    int n, m, s, d;  //n = astro objects, m = links, s = starting loc, d = destinations
    std::cin>>n>>m>>s>>d;

    graph G(2*n);
    edge_adder adder(G);

    //Connect the two parts of each vertex
    for(int i = 0; i < n; i++){
      adder.add_edge(2*i, 2*i+1, 1);
    }

    //Connect odd part of the source vertex to the even part of the target vertex
    for(int i = 0; i < m; i++){
      int x, y;
      std::cin>>x>>y;

      adder.add_edge(2*x+1, 2*y, 1);
    }

    const vertex_desc source = boost::add_vertex(G);
    const vertex_desc sink = boost::add_vertex(G);

    //Connect source to even part of the starting vertices
    for(int i = 0; i < s; i++){
      int x;
      std::cin>>x;

      adder.add_edge(source, 2*x, INT_MAX);
    }

    //Connect odd part of the destination vertices to the sink
    for(int i = 0; i < d; i++){
      int x;
      std::cin>>x;

      adder.add_edge(2*x+1, sink, INT_MAX);
    }

    //By dividing in two the vertices, it's like doing a min cut on the vertices
    long flow = boost::push_relabel_max_flow(G, source, sink);
    std::cout << flow << "\n";
  }
}
