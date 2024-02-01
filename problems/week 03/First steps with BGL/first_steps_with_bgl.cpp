#include <iostream>
#include <vector>
#include <algorithm>

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
    int n, m;  //n = nodes, m = edges
    std::cin>>n>>m;

    graph G(n);
    weight_map weights = boost::get(boost::edge_weight, G);

    for(int i = 0; i < m; i++){
      int u, v, w;
      std::cin>>u>>v>>w;

      edge_desc e = boost::add_edge(u, v, G).first;
      weights[e] = w;
    }

    std::vector<edge_desc> mst;
    boost::kruskal_minimum_spanning_tree(G, std::back_inserter(mst));

    int sum = 0;
    for (auto it = mst.begin(); it != mst.end(); it++) {
      sum += weights[*it];
    }

    std::vector<int> dist_map(n);
    boost::dijkstra_shortest_paths(G, 0,
      boost::distance_map(boost::make_iterator_property_map(
        dist_map.begin(), boost::get(boost::vertex_index, G))));

    int max_d = -1;
    for(int i = 0; i < n; i++){
      max_d = std::max(max_d, dist_map[i]);
    }

    std::cout<<sum<<" "<<max_d<<"\n";
  }
}
