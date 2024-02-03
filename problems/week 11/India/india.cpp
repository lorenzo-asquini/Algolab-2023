#include <iostream>
#include <climits>
#include <algorithm>
#include <vector>

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
typedef traits::edge_descriptor edge_desc;

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

typedef struct{
  int x;
  int y;
  int d;
  int e;
} guide_t;

long get_cost(std::vector<guide_t> &guides, int current_luggages, int c, int k, int a){
  graph G(c+2);
  edge_adder adder(G);

  for(guide_t guide: guides){
    adder.add_edge(guide.x, guide.y, guide.e, guide.d);
  }

  const int source = c;
  const int sink = c+1;

  adder.add_edge(source, k, current_luggages, 0);
  adder.add_edge(a, sink, current_luggages, 0);

  int flow = boost::push_relabel_max_flow(G, source, sink);
  boost::successive_shortest_path_nonnegative_weights(G, source, sink);
  int cost = boost::find_flow_cost(G);

  //It may be possible for the cost to be low only because not all suitcases are sent
  //If so, increase the cost to make it try less suitcases next time
  if(flow < current_luggages){
    cost = INT_MAX;
  }

  return cost;
}

int main(){
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin>>t;
  while(t--){
    int c, g, b, k, a;  //c = cities, g = guides, b = budget, k = start, a = end
    std::cin>>c>>g>>b>>k>>a;

    graph G(c);
    edge_adder adder(G);

    int max_luggages = 0;
    std::vector<guide_t> guides(g);
    for(int i = 0; i < g; i++){
      int x, y, d, e;
      std::cin>>x>>y>>d>>e;

      guides[i] = {x, y, d, e}; //Guide i has e elephants that go from x to y, with cost d for each one
      max_luggages += e;
    }

    //There is a looooot of time, so it's possible to do binary search on
    //the number of luggages that can be used.
    //For each try, add or remove luggages depending if the cost is lower or higher than the budget

    //The maximum number of suitcases that can be transported is equal to the number of elephants
    long low = 0, high = max_luggages;
    long res = -1;

    while(low <= high){
      long mid = (low+high)/2;

      int cost = get_cost(guides, mid, c, k, a);

      if(cost > b){
        high = mid-1;

      }else if(cost < b){
        low = mid+1;
        res = std::max(res, mid);

      }else{
        res = mid;
        break;
      }
    }

    std::cout<<res<<"\n";
  }
}
