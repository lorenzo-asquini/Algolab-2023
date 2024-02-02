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

typedef std::vector<int> VI;
typedef std::vector<VI> VVI;

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
    int n;  //n = side of the chessboard
    std::cin>>n;

    VVI table(n, VI(n));
    int tot_k = 0;
    for(int i = 0; i < n; i++){
      for(int j = 0; j < n; j++){
        int k;
        std::cin>>k;

        table[i][j] = k;

        if(k == 1){
          tot_k++;
        }
      }
    }

    graph G(n*n);
    edge_adder adder(G);

    //Connect all starting positions to source and all landing positions to sink
    const vertex_desc source = boost::add_vertex(G);
    const vertex_desc sink = boost::add_vertex(G);

    for(int i = 0; i < n; i++){
      for(int j = 0; j < n; j++){
        int k = table[i][j];
        int node = i*n + j;

        if(k == 1){  //If field is not a hole
          if((i+j)%2 == 0){
            adder.add_edge(source, node, 1);

            //Add an edge to all possible landing positions
            //If the landing field is not a hole

            //i-1, j-2
            if(i-1 >= 0 && j-2 >= 0 && table[i-1][j-2] == 1){
              int other = (i-1)*n + (j-2);
              adder.add_edge(node, other, 1);
            }

            //i-1, j+2
            if(i-1 >= 0 && j+2 < n && table[i-1][j+2] == 1){
              int other = (i-1)*n + (j+2);
              adder.add_edge(node, other, 1);
            }

            //i+1, j-2
            if(i+1 < n && j-2 >= 0 && table[i+1][j-2] == 1){
              int other = (i+1)*n + (j-2);
              adder.add_edge(node, other, 1);
            }

            //i+1, j+2
            if(i+1 < n && j+2 < n && table[i+1][j+2] == 1){
              int other = (i+1)*n + (j+2);
              adder.add_edge(node, other, 1);
            }

            //i-2, j-1
            if(i-2 >= 0 && j-1 >= 0 && table[i-2][j-1] == 1){
              int other = (i-2)*n + (j-1);
              adder.add_edge(node, other, 1);
            }

            //i-2, j+1
            if(i-2 >= 0 && j+1 < n && table[i-2][j+1] == 1){
              int other = (i-2)*n + (j+1);
              adder.add_edge(node, other, 1);
            }

            //i+2, j-1
            if(i+2 < n && j-1 >= 0 && table[i+2][j-1] == 1){
              int other = (i+2)*n + (j-1);
              adder.add_edge(node, other, 1);
            }

            //i+2, j+1
            if(i+2 < n && j+1 < n && table[i+2][j+1] == 1){
              int other = (i+2)*n + (j+1);
              adder.add_edge(node, other, 1);
            }
          }else{
            adder.add_edge(node, sink, 1);
          }
        }
      }
    }

    long flow = boost::push_relabel_max_flow(G, source, sink);
    //Maximum flow = Maximum Matching = Minimum Vertex Cover
    //Maximum Independent Set = Total Vertices - Minimum Vertex Cover
    std::cout<<tot_k-flow<<"\n";
  }
}
