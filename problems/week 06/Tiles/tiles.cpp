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

  int n;
  std::cin>>n;
  while(n--){
    int w, h;
    std::cin>>w>>h;

    int to_cover = 0;

    std::vector<std::vector<char>> board(h, std::vector<char>(w));
    for(int i = 0; i < h; i++){
      for(int j = 0; j < w; j++){
        char c;
        std::cin>>c;

        if(c == '.'){
          to_cover++;
        }

        board[i][j] = c;
      }
    }

    //It's not possible to cover an odd number of spaces
    if(to_cover % 2 == 1){
      std::cout<<"no\n";
      continue;
    }

    graph G(w*h);
    edge_adder adder(G);

    const vertex_desc v_source = boost::add_vertex(G);
    const vertex_desc v_sink = boost::add_vertex(G);

    for(int i = 0; i < h; i++){
      for(int j = 0; j < w; j++){
        char c = board[i][j];

        //If the current space needs to be tiled
        if(c == '.'){
          int node = i*w+j;

          //Each space to be tiled can only be connected to the source or the sink
          if((i+j) % 2 == 0){

            adder.add_edge(v_source, node, 1);

            if(i > 0 && board[i-1][j] == '.'){
              int other_node = (i-1)*w+j;
              adder.add_edge(node, other_node, 1);
            }

            if(i < h-1 && board[i+1][j] == '.'){
              int other_node = (i+1)*w+j;
              adder.add_edge(node, other_node, 1);
            }

            if(j > 0 && board[i][j-1] == '.'){
              int other_node = i*w+(j-1);
              adder.add_edge(node, other_node, 1);
            }

            if(j < w-1 && board[i][j+1] == '.'){
              int other_node = i*w+(j+1);
              adder.add_edge(node, other_node, 1);
            }
          }else{
            adder.add_edge(node, v_sink, 1);
          }
        }
      }
    }

    long flow = boost::push_relabel_max_flow(G, v_source, v_sink);

    if(flow == to_cover/2){
      std::cout<<"yes\n";
    }else{
      std::cout<<"no\n";
    }
  }
}
