#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>

#include <boost/pending/disjoint_sets.hpp>

typedef struct{
  int from;
  int to;
  int cost;
} edge_t;

//Cheaper edges first
struct compEdges{
  bool operator()(edge_t &e1, edge_t &e2){
    return e1.cost < e2.cost;
  }
};

int main(){
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin>>t;
  while(t--){
    int n, tatooine; //n = planets, tatooine = its index
    std::cin>>n>>tatooine;

    std::vector<edge_t> edges;
    for(int j = 0; j < n; j++){
      for(int k = j+1; k < n; k++){
        int x;
        std::cin>>x;

        edges.push_back({j, k, x});
      }
    }

    std::sort(edges.begin(), edges.end(), compEdges());

    //Best cost of the second MST
    int best_cost = INT_MAX;

    //Skip one edge at a time
    for(int to_skip = 0; to_skip < n-1; to_skip++){

      //Add edges until all nodes are connected, starting from the cheapest edges
      //Consider edges only if they connect two differenct connected components
      boost::disjoint_sets_with_storage<> mst(n);
      int n_components = n;

      int tot_cost = 0;
      bool skipped = false;

      for(int j = 0; j < (int)edges.size() && n_components > 1; j++){
        edge_t e = edges[j];

        int c1 = mst.find_set(e.from);
        int c2 = mst.find_set(e.to);

        if (c1 != c2) {
          //It's the same thing trying to skip, one by one, all the edges in the first MST
          //And using another edge to connect the connected components that would have been connected
          //By the skipped edge
          if(!skipped && n-n_components == to_skip){
            skipped = true;

          }else{
            tot_cost += e.cost;
            mst.link(c1, c2);
            n_components--;
          }
        }
      }

      best_cost = std::min(best_cost, tot_cost);
    }

    std::cout<<best_cost<<"\n";
  }
}
