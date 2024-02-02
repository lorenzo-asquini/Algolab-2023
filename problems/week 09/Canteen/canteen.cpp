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

typedef boost::graph_traits<graph>::edge_descriptor edge_desc;

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

  int t;
  std::cin>>t;
  while(t--){
    int n;  //n = number of days
    std::cin>>n;

    graph G(n);
    edge_adder adder(G);

    const int source = n;
    const int sink = source+1;

    for(int i = 0; i < n; i++){
      int a, c;  //a = amount of menus, c = cost per menu
      std::cin>>a>>c;

      //For each day it's possible to produce a menus with cost c
      adder.add_edge(source, i, a, c);
    }

    int tot_s = 0;
    for(int i = 0; i < n; i++){
      int s, p;  //s = number of students, p = price imposed
      std::cin>>s>>p;

      tot_s += s;

      //For each day, it's possible to serve at most s students, with a revenue of p for each menu (cost is -p)
      //Transform negative cost in a positive cost to use a more efficient function. At the end it's necessary to fix the result
      adder.add_edge(i, sink, s, 20-p);
    }

    for(int i = 0; i < n-1; i++){
      int v, e;  //v = max menus in freezer, e = energy cost per menu
      std::cin>>v>>e;

      //It's possible to transfer a maximum of v menus from one day to another with cost e per menu
      adder.add_edge(i, i+1, v, e);
    }

    //The flow is the amount of students served
    int students = boost::push_relabel_max_flow(G, source, sink);

    //Possible to use after transforming the negative costs
    boost::successive_shortest_path_nonnegative_weights(G, source, sink);

    //Profit is the negative of the remaining cost
    int profit = 20*students - boost::find_flow_cost(G);

    if(students >= tot_s){
      std::cout<<"possible ";
    }else{
      std::cout<<"impossible ";
    }

    std::cout<<students<<" "<<profit<<"\n";
  }
}
