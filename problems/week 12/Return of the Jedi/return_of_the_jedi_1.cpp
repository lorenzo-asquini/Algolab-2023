#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
  boost::no_property, boost::property<boost::edge_weight_t, int>> graph;

typedef boost::property_map<graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<graph>::edge_descriptor edge_desc;

int main(){
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin>>t;
  while(t--){
    int n, tatooine; //n = planets, tatooine = its index
    std::cin>>n>>tatooine;

    graph G(n);
    weight_map weights = boost::get(boost::edge_weight, G);

    //Save the costs of the indirect edges (so, both ways)
    std::vector<std::vector<int>> costs(n, std::vector<int>(n));

    edge_desc e;
    for(int j = 0; j < n-1; j++){
      for(int k = 0; k < n-j-1; k++){
        int c;
        std::cin>>c;

        e = boost::add_edge(j, j+k+1, G).first;
        weights[e] = c;

        costs[j][j+k+1] = c;
        costs[j+k+1][j] = c;
      }
    }

    //Compute MST of the original graph, that is the proposed network
    std::vector<edge_desc> mst;
    boost::kruskal_minimum_spanning_tree(G, std::back_inserter(mst));

    //Create a matrix with vectors to represent the mst and mark what edges are inside the mst
    int mst_cost = 0;
    std::vector<std::vector<int>> neighbors(n);
    std::vector<std::vector<int>> edge_in_mst(n, std::vector<int>(n, false));

    for (auto edge : mst) {
      int u = boost::source(edge, G);
      int v = boost::target(edge, G);

      mst_cost += weights[edge];

      neighbors[u].push_back(v);
      neighbors[v].push_back(u);

      edge_in_mst[u][v] = true;
      edge_in_mst[v][u] = true;
    }

    //Find the most expensive edge in each path u-v in the mst
    std::vector<std::vector<int>> most_expensive(n, std::vector<int>(n, 0));

    for(int u = 0; u < n; u++){

      //Starting from a node u, connect to all the possible nodes using DFS
      std::vector<int> stack;
      stack.push_back(u);

      std::vector<int> visited(n, false);
      visited[u] = true;

      while (!stack.empty()){

        int v = *stack.rbegin();
        stack.pop_back();

        //The maximum cost of going from u to w is the maximum between
        //the maximum cost of going from u to v, and the cost of going from v to w
        for(auto w : neighbors[v]){

          if (!visited[w]) {
            visited[w] = true;
            most_expensive[u][w] = std::max(costs[v][w], most_expensive[u][v]);
            stack.push_back(w);
          }
        }
      }
    }

    //For all edges e = {u, v}, that are not in the MST, we will try adding e to the MST.
    //This will introduce a cycle. We remove the most expensive edge of the cycle that is
    //part of the MST. For this we can simply query the most expensive edge from u to v in the MST
    //and remove it. The cost of the MST changes by delta = cost[u][v] - cost[most expensive edge].
    //We simply find the best delta
    int min_increase = INT_MAX;

    for (int u = 0; u < n; u++){
      for (int v = u + 1; v < n; v++){

        if (!edge_in_mst[u][v]){
          int val = costs[u][v] - most_expensive[u][v];
          min_increase = std::min(val, min_increase);
        }
      }
    }

    std::cout<< mst_cost + min_increase <<"\n";
  }
}
