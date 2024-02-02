#include <iostream>
#include <vector>

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
    int l, p; //l = locations, p = paths
    std::cin>>l>>p;

    std::vector<int> supply(l);  //Positive if supply, negative number if demand
    for(int i = 0; i < l; i++){
      int s, d;
      std::cin>>s>>d;

      supply[i] = s-d;
    }

    graph G(l);
    edge_adder adder(G);

    for(int i = 0; i < p; i++){
      int from, to, min_c, max_c;
      std::cin>>from>>to>>min_c>>max_c;

      //capacity of the edge is the difference between the maximum and the minimum capacity
      adder.add_edge(from, to, max_c-min_c);

      supply[from] -= min_c;  //Increase demand (decrease supply) of from
      supply[to] += min_c;  //Increase supply of to
    }

    const int source = l;
    const int sink = source+1;

    int tot_demand = 0;
    for(int i = 0; i < l; i++){
      //Connect supply to source and demand to sink
      if(supply[i] > 0){
        adder.add_edge(source, i, supply[i]);
      }else{
        tot_demand += -supply[i];
        adder.add_edge(i, sink, -supply[i]);
      }
    }

    long flow = boost::push_relabel_max_flow(G, source, sink);
    if(flow == tot_demand){
      std::cout<<"yes\n";
    }else{
      std::cout<<"no\n";
    }
  }
}
