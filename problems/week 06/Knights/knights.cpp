#include <iostream>
#include <vector>
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
    int m, n, k, c;
    std::cin>>m>>n>>k>>c;

    graph G(2*m*n);
    edge_adder adder(G);

    const vertex_desc v_source = boost::add_vertex(G);
    const vertex_desc v_sink = boost::add_vertex(G);

    for(int i = 0; i < n; i++){
      for(int j = 0; j < m; j++){
        int node = i*m+j;

        //Each vertex represents an intersection, that can be used c times
        adder.add_edge(2*node, 2*node+1, c);

        //Connect the intersections with tunnels or connect them to the exit
        if(i >= 1){
          int other = (i-1)*m + j;
          adder.add_edge(2*node+1, 2*other, 1);
        }else{
          adder.add_edge(2*node+1, v_sink, 1);
        }

        if(i <= n-2){
          int other = (i+1)*m + j;
          adder.add_edge(2*node+1, 2*other, 1);
        }else{
          adder.add_edge(2*node+1, v_sink, 1);
        }

        if(j >= 1){
          int other = i*m + j - 1;
          adder.add_edge(2*node+1, 2*other, 1);
        }else{
          adder.add_edge(2*node+1, v_sink, 1);
        }

        if(j <= m-2){
          int other = i*m + j + 1;
          adder.add_edge(2*node+1, 2*other, 1);
        }else{
          adder.add_edge(2*node+1, v_sink, 1);
        }
      }
    }

    for(int i = 0; i < k; i++){
      int x, y;
      std::cin>>x>>y;

      //Connect each knight to the intersection it is in
      int node = y*m+x;
      adder.add_edge(v_source, 2*node, 1);
    }

    long flow = boost::push_relabel_max_flow(G, v_source, v_sink);
    std::cout<<flow<<"\n";
  }
}
