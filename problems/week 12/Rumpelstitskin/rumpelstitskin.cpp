#include <iostream>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor,
                boost::property <boost::edge_weight_t, long>>>>> graph; // new! weightmap corresponds to costs

typedef boost::graph_traits<graph>::edge_descriptor edge_desc;

class edge_adder {
  graph &G;

  public:
    explicit edge_adder(graph &G) : G(G) {}
    void add_edge(int from, int to, long capacity, long cost) {
      auto c_map = boost::get(boost::edge_capacity, G);
      auto r_map = boost::get(boost::edge_reverse, G);
      auto w_map = boost::get(boost::edge_weight, G);
      const edge_desc e = boost::add_edge(from, to, G).first;
      const edge_desc rev_e = boost::add_edge(to, from, G).first;
      c_map[e] = capacity;
      c_map[rev_e] = 0;
      r_map[e] = rev_e;
      r_map[rev_e] = e;
      w_map[e] = cost;
      w_map[rev_e] = -cost;
    }
};

int main(){
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin>>t;
  while(t--){
    int n;  //n = islands
    int pA, pB;  //cities in network X
    int eA, eB;  //edges in network X
    int fA, fB;  //edges from X to the islands
    int sA, sB;  //sacks in the network X
    std::cin>>n>>pA>>pB>>eA>>eB>>fA>>fB>>sA>>sB;

    //Represent each island with two nodes,
    //connected with an edge of capacity 1 (a spinner can be used only once)
    //and cost the opposite of the preference (shifted up to use the best algorithm)
    graph G(2*n+pA+pB+2);
    edge_adder adder(G);

    const int source = 2*n+pA+pB;
    const int sink = source+1;

    //All the resources are contained in the castles of the two kingdoms
    adder.add_edge(source, 2*n, sA, 0);
    adder.add_edge(2*n+pA, sink, sB, 0);

    //Islands from 0 to 2n-1
    for(int i = 0; i < n; i++){
      int a;
      std::cin>>a;

      //Can use each island only once
      adder.add_edge(2*i, 2*i+1, 1, 1024 - a);
    }

    //Network A: from 2*n to 2*n+pA-1
    for(int i = 0; i < eA; i++){
      int u, v, c;
      std::cin>>u>>v>>c;

      adder.add_edge(2*n+u, 2*n+v, c, 0);
    }

    //Network B: from 2*n+pA to 2*n+pA+pB-1
    //The flow will go from network A to network B through the islands, so it's necessary to invert the verse of the edges
    for(int i = 0; i < eB; i++){
      int u, v, c;
      std::cin>>u>>v>>c;

      adder.add_edge(2*n+pA+v, 2*n+pA+u, c, 0);
    }

    //From network A to islands
    for(int i = 0; i < fA; i++){
      int u, v, c;
      std::cin>>u>>v>>c;

      adder.add_edge(2*n+u, 2*v, c, 0);
    }

    //From the islands to network B
    for(int i = 0; i < fB; i++){
      int u, v, c;
      std::cin>>u>>v>>c;

      adder.add_edge(2*v+1, 2*n+pA+u, c, 0);
    }

    int flow = boost::push_relabel_max_flow(G, source, sink);
    boost::successive_shortest_path_nonnegative_weights(G, source, sink);
    int cost = boost::find_flow_cost(G);

    std::cout<<flow<<" "<<1024*flow - cost<<"\n";
  }
}
