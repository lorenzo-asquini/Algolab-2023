#include <iostream>
#include <climits>
#include <algorithm>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
  boost::property<boost::edge_capacity_t, long,
    boost::property<boost::edge_residual_capacity_t, long,
      boost::property<boost::edge_reverse_t, traits::edge_descriptor>>>> graph;

typedef boost::graph_traits<graph>::out_edge_iterator out_edge_it;
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
    int n, m; //n = figures, m = limbs
    std::cin>>n>>m;

    graph G(n);
    edge_adder adder(G);

    std::vector<std::pair<int, int>> costs(n);
    for(int i = 0; i < m; i++){
      int a, b, c;
      std::cin>>a>>b>>c;  //From a to b with cost c

      adder.add_edge(a, b, c);
    }

    //Find the minimum cut (= maximum flow) for each possible division
    //It does not matter with node in S or T is chosen (but in T it's necessary
    //to choose the first one to have a division)
    //It's necessary to consider both directions
    long flow = LONG_MAX;
    for(int i = 1; i < n; i++) {
      flow = std::min(flow, boost::push_relabel_max_flow(G, 0, i));
      flow = std::min(flow, boost::push_relabel_max_flow(G, i, 0));
    }

    std::cout<<flow<<std::endl;
  }
}
