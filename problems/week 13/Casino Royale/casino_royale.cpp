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
                boost::property <boost::edge_weight_t, long>>>>> graph;

typedef traits::vertex_descriptor vertex_desc;
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

typedef struct {
  int from;
  int to;
  int priority;
} mission_t;

int main(){
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin>>t;
  while(t--){
    int n, m, l; //n = train stops, m = missions, l = max in train
    std::cin>>n>>m>>l;

    graph G(n);
    edge_adder adder(G);

    const int source = n;
    const int sink = n-1;  //The sink is the last station

    //Connect the source to the first station
    adder.add_edge(source, 0, l, 0);

    int max_priority = 128;

    for(int i = 0; i < n-1; i++){
      //Connect the stations to allow carry on unused capacity
      //The cost cannot be zero otherwise the function would prefer not doing missions
      adder.add_edge(i, i+1, l, max_priority);
    }

    for(int i = 0; i < m; i++){
      int x, y, q;
      std::cin>>x>>y>>q;

      //Doing a mission skips (y-x) stations and collects q points
      adder.add_edge(x, y, 1, (y-x)*max_priority - q);
    }

    //Possible to use after transforming the negative costs
    boost::successive_shortest_path_nonnegative_weights(G, source, sink);

    //The base cost comes from using the empty trains: (n-1) connections, l passengers
    int k = l*(n-1)*max_priority - boost::find_flow_cost(G);

    std::cout<<k<<"\n";
  }
}
