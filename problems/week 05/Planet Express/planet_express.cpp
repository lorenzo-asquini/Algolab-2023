#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/strong_components.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
  boost::no_property, boost::property<boost::edge_weight_t, int>> graph;

typedef boost::property_map<graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<graph>::edge_descriptor edge_desc;

int main(){
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin>>t;
  while(t--){
    int n, m, k, T; //n = vertices, m = edges, k = warehouses, T = teleport planets
    std::cin>>n>>m>>k>>T;

    std::vector<int> teleport_planets(T);
    for(int i = 0; i < T; i++){
      int p;
      std::cin>>p;

      teleport_planets[i] = p;
    }

    graph G(n);
    weight_map weights = boost::get(boost::edge_weight, G);

    //Create the graph in reverse to speed up Dijkstra
    for(int i = 0; i < m; i++){
      int u, v, c;
      std::cin>>u>>v>>c;

      edge_desc e = boost::add_edge(v, u, G).first;
      weights[e] = c;
    }

    //Set up the teleportation network
    std::vector<int> scc_map(n);

    int nscc = boost::strong_components(G,
      boost::make_iterator_property_map(scc_map.begin(), boost::get(boost::vertex_index, G)));

    //A single teleportation network is formed by all the planets with teleportation
    //capabilities that are in the same strong connected component
    std::vector<int> teleport_per_scc(nscc, 0);
    for(int i = 0; i < T; i++){
      int tp_planet = teleport_planets[i];
      teleport_per_scc[scc_map[tp_planet]]++;
    }

    //Connect each teleport planet to a strong component hub
    //One way is free, the other costs the right amount
    for(int i = 0; i < T; i++){
      int tp_planet = teleport_planets[i];
      int scc = scc_map[tp_planet];

      edge_desc e = boost::add_edge(tp_planet, n+scc, G).first;
      weights[e] = 0;

      e = boost::add_edge(n+scc, tp_planet, G).first;
      weights[e] = teleport_per_scc[scc]-1;
    }

    //It's much faster to calculate the distance from the end point
    int vertices = boost::num_vertices(G);
    std::vector<int> dist_map(vertices);

    boost::dijkstra_shortest_paths(G, n-1,
      boost::distance_map(boost::make_iterator_property_map(
        dist_map.begin(), boost::get(boost::vertex_index, G))));

    int best_dist = INT_MAX;
    for(int p = 0; p < k; p++){
      best_dist = std::min(best_dist, dist_map[p]);
    }

    if(best_dist <= 1000000){
      std::cout<<best_dist<<"\n";
    }else{
      std::cout<<"no\n";
    }
  }
}
