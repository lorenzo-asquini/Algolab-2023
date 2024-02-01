#include <iostream>
#include <vector>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
  boost::no_property, boost::property<boost::edge_weight_t, int>> graph;

typedef boost::property_map<graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<graph>::edge_descriptor edge_desc;

int main(){
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin>>t;
  while(t--){
    int n, e, s, a, b;  //n = nodes, e = edges, s = species
    std::cin>>n>>e>>s>>a>>b;

    std::vector<graph> full_networks(s);
    std::vector<weight_map> weights(s);

    //Create the full graphs for all the networks of the different species
    for(int i = 0; i < s; i++){
      full_networks[i] = graph(n);
      weights[i] = boost::get(boost::edge_weight, full_networks[i]);
    }

    for(int i = 0; i < e; i++){
      int t1, t2;
      std::cin>>t1>>t2;

      for(int j = 0; j < s; j++){
        int w;
        std::cin>>w;

        edge_desc e = boost::add_edge(t1, t2, full_networks[j]).first;
        weights[j][e] = w;
      }
    }

    for(int i = 0; i < s; i++){
      int h;
      std::cin>>h;  //Useless
    }

    //Will contain the edges of all the fastest networks of the different species
    graph fast_network(n);
    weight_map fast_weights = boost::get(boost::edge_weight, fast_network);

    //Find the MST of each specie and add its edges to the final network
    for(int i = 0; i < s; i++){

      std::vector<edge_desc> mst;
      boost::kruskal_minimum_spanning_tree(full_networks[i], std::back_inserter(mst));

      for(auto it = mst.begin(); it != mst.end(); it++){
        int u = boost::source(*it, full_networks[i]);
        int v = boost::target(*it, full_networks[i]);

        edge_desc e = boost::add_edge(u, v, fast_network).first;
        fast_weights[e] = weights[i][*it];
      }
    }

    std::vector<int> dist_map(n);

    //Find the shortest path to the destination choosing edges from the fast networks of the different species
    boost::dijkstra_shortest_paths(fast_network, a,
      boost::distance_map(boost::make_iterator_property_map(
        dist_map.begin(), boost::get(boost::vertex_index, fast_network))));

    std::cout<<dist_map[b]<<"\n";
  }
}
