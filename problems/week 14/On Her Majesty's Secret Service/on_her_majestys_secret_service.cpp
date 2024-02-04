#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/max_cardinality_matching.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> un_graph;

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
  boost::no_property, boost::property<boost::edge_weight_t, long>> graph;

typedef boost::property_map<graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<graph>::edge_descriptor edge_desc;
typedef boost::graph_traits<graph>::vertex_descriptor  vertex_desc;

typedef struct{
  int x;
  int y;
  long dist;
} edge_t;

int main(){
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin>>t;
  while(t--){
    //n = relevant position, m = slopes and ski lifts, a = agents
    //s = shelters, c = capacity, d = enter time
    int n, m, a, s, c, d;
    std::cin>>n>>m>>a>>s>>c>>d;

    graph G(n);
    weight_map weights = boost::get(boost::edge_weight, G);

    edge_desc e;
    for(int i = 0; i < m; i++){
      char w; int x, y, z;
      std::cin>>w>>x>>y>>z;

      if(w == 'S'){
        e = boost::add_edge(x, y, G).first;
        weights[e] = z;
      }else{
        //With a lift it's possible to go both ways
        e = boost::add_edge(x, y, G).first;
        weights[e] = z;

        e = boost::add_edge(y, x, G).first;
        weights[e] = z;
      }
    }

    std::vector<int> agents(a);
    for(int i = 0; i < a; i++){
      int x;
      std::cin>>x;

      agents[i] = x;
    }

    std::vector<int> shelters(s);
    for(int i = 0; i < s; i++){
      int x;
      std::cin>>x;

      shelters[i] = x;
    }

    //Find all the possible routes from an agent to a shelter
    //Double route and shelter if capacity is 2
    std::vector<edge_t> edges;
    for(int i = 0; i < a; i++){
      std::vector<int> dist_map(n);

      boost::dijkstra_shortest_paths(G, agents[i],
        boost::distance_map(boost::make_iterator_property_map(
          dist_map.begin(), boost::get(boost::vertex_index, G))));

      for(int j = 0; j < s; j++){
        long dist = dist_map[shelters[j]];

        if(dist == INT_MAX){
          continue;
        }

        //It may be possible to add a double link to a shelter if it allows a second agent to reach it in time
        for(int k = 1; k <= c; k++) {
          //agents -> from 0 to a-1
          //shelters -> from a to a+2*s-1
          edges.push_back({i, a + (k-1)*s + j, dist + k*d});
        }
      }
    }

    //Create iteratively a graph with only edges that allows
    //an agent to enter the shelter (movement+password) in less than t
    long low = 0; long high = INT_MAX;
    while(low <= high){
      long mid = (low+high)/2;

      //In the new graph there are a node for each agent, and a node for each shelter
      //It's necessary to multiply the shelters by c to consider also the case when multiple agents use the same shelter
      un_graph best(a+c*s);

      for(edge_t path: edges){
        if(path.dist <= mid){
          boost::add_edge(path.x, path.y, best);
        }
      }

      //Find if all agents can have a shelter
      std::vector<vertex_desc> mate_map(a+c*s);

      //Create a cadinality matching to see if there is at lest a path between an agent and a shelter
      boost::edmonds_maximum_cardinality_matching(best,
        boost::make_iterator_property_map(mate_map.begin(), boost::get(boost::vertex_index, best)));

      int matching_size = boost::matching_size(best,
        boost::make_iterator_property_map(mate_map.begin(), boost::get(boost::vertex_index, best)));

      if(matching_size == a){  //There is a path for each agent
        high = mid-1;
      }else{  //The timing is too strict, it's necessary to increase it
        low = mid+1;
      }
    }

    std::cout<<low<<"\n";
  }
}
