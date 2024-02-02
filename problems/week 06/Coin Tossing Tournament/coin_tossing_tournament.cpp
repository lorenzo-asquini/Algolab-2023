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

typedef struct{
  int a;
  int b;
  int c;
} round_t;

int main(){
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin>>t;
  while(t--){
    int n, m;  //n = players, m = rounds
    std::cin>>n>>m;

    std::vector<round_t> rounds(m);
    for(int i = 0; i < m; i++){
      int a, b, c;  //c = 1 -> a wins, c = 2 -> b wins, c = 0 -> no idea
      std::cin>>a>>b>>c;

      rounds[i] = {a, b, c};
    }

    std::vector<int> scores(n);
    int sum_scores = 0;
    for(int i = 0; i < n; i++){
      int s;
      std::cin>>s;

      scores[i] = s;
      sum_scores += s;
    }

    graph G(m+n);
    edge_adder adder(G);

    const vertex_desc v_source = boost::add_vertex(G);
    const vertex_desc v_sink = boost::add_vertex(G);

    for(int i = 0; i < m; i++){
      //One point to assign this round
      adder.add_edge(v_source, i, 1);

      if(rounds[i].c == 1){
        //Assign to a only
        adder.add_edge(i, m+rounds[i].a, 1);
      }else if(rounds[i].c == 2){
        //Assign to b only
        adder.add_edge(i, m+rounds[i].b, 1);
      }else{
        //Assign to one of the two
        adder.add_edge(i, m+rounds[i].a, 1);
        adder.add_edge(i, m+rounds[i].b, 1);
      }
    }

    for(int i = 0; i < n; i++){
      //Connect each player to the sink with the score it should get
      adder.add_edge(m+i, v_sink, scores[i]);
    }

    long flow = boost::push_relabel_max_flow(G, v_source, v_sink);

    //If all the points are assigned and the final total score is correct
    if(flow == m && sum_scores == m){
      std::cout<<"yes\n";
    }else{
      std::cout<<"no\n";
    }
  }
}
