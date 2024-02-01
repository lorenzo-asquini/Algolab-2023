#include <iostream>
#include <vector>
#include <set>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/biconnected_components.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
  boost::no_property, boost::property<boost::edge_name_t, int>> graph;

typedef boost::property_map<graph, boost::edge_name_t>::type component_map;
typedef boost::graph_traits<graph>::edge_iterator edge_it;

int main(){
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin>>t;
  while(t--){
    int n, m;
    std::cin>>n>>m;

    graph G(n);

    for(int i = 0; i < m; i++){
      int u, v;
      std::cin>>u>>v;

      boost::add_edge(u, v, G);
    }

    component_map component = boost::get(boost::edge_name, G);
    int num_comps = boost::biconnected_components(G, component);

    //Find how many bridges there are in each biconnected component
    std::vector<int> num_bridges(num_comps, 0);
    edge_it ei, ei_end;
    for (boost::tie(ei, ei_end) = boost::edges(G); ei != ei_end; ei++){
      num_bridges[component[*ei]]++;
    }

    //Important bridges in biconnected components of size 1
    int important_bridges = std::count(num_bridges.begin(), num_bridges.end(), 1);
    std::cout<<important_bridges<<"\n";

    //Find and order the important bridges
    std::set<std::pair<int, int>> imp;
    for (boost::tie(ei, ei_end) = boost::edges(G); ei != ei_end; ei++){
     bool is_important = (num_bridges[component[*ei]] == 1);

     int u = std::min(boost::source(*ei, G), boost::target(*ei, G));
     int v = std::max(boost::source(*ei, G), boost::target(*ei, G));

     if(is_important){
      imp.insert({u, v});
     }
    }

    for(auto e: imp){
      std::cout<<e.first<<" "<<e.second<<"\n";
    }
  }
}
