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

typedef traits::edge_descriptor edge_desc;
typedef traits::vertex_descriptor vertex_desc;

class edge_adder {
  graph &G;

  public:
    explicit edge_adder(graph &G) : G(G) {}
    void add_edge(int from, int to, long capacity, long cost) {
      auto c_map = boost::get(boost::edge_capacity, G);
      auto r_map = boost::get(boost::edge_reverse, G);
      auto w_map = boost::get(boost::edge_weight, G); // new!
      const edge_desc e = boost::add_edge(from, to, G).first;
      const edge_desc rev_e = boost::add_edge(to, from, G).first;
      c_map[e] = capacity;
      c_map[rev_e] = 0; // reverse edge has no capacity!
      r_map[e] = rev_e;
      r_map[rev_e] = e;
      w_map[e] = cost;   // new assign cost
      w_map[rev_e] = -cost;   // new negative cost
    }
};

int main(){
  std::ios_base::sync_with_stdio(false);

  int T;
  std::cin>>T;
  while(T--){
    int N, M, S;  //N = buyers, M = properties, S = states
    std::cin>>N>>M>>S;

    graph G(S+M+N);
    edge_adder adder(G);

    const vertex_desc source = boost::add_vertex(G);
    const vertex_desc sink = boost::add_vertex(G);

    //States from 0 to S-1
    for(int i = 0; i < S; i++){
      int l;
      std::cin>>l;

      //Limit the amount of properties that can be bought in a state
      adder.add_edge(source, i, l, 0);
    }

    //Properties from S to S+M-1
    for(int i = S; i < S+M; i++){
      int s;
      std::cin>>s;

      //Each property is in a state
      adder.add_edge(s-1, i, 1, 0);
    }

    const int max_bid = 100;
    //Buyers from S+M to S+M+N-1
    for(int i = S+M; i < S+M+N; i++){
      //Each buyer buys one property
      adder.add_edge(i, sink, 1, 0);
      for(int j = S; j < S+M; j++){
        int b;
        std::cin>>b;

        //Shift up the bid
        adder.add_edge(j, i, 1, max_bid-b);
      }
    }

    int prop_sold = boost::push_relabel_max_flow(G, source, sink);
    boost::successive_shortest_path_nonnegative_weights(G, source, sink);
    int profit = prop_sold*max_bid - boost::find_flow_cost(G);

    std::cout<<prop_sold<<" "<<profit<<"\n";
  }
}
