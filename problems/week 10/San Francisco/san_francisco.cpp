#include <iostream>
#include <vector>
#include <algorithm>

typedef struct{  //The source node is the index in a vector
  int to;
  long p;
} edge_t;

int main(){
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin>>t;
  while(t--){
    long n, m, x, k; //n = holes, m = canals, x = claimed score, k = max moves
    std::cin>>n>>m>>x>>k;

    std::vector<std::vector<edge_t>> graph(n);
    for(int i = 0; i < m; i++){
      int u, v, p;
      std::cin>>u>>v>>p;

      graph[u].push_back({v, p});
    }

    //Find which one is the end hole and connect it to the start with 0 points
    int end_v = -1;
    for(int i = 0; i < n; i++){
      if(graph[i].size() == 0){
        end_v = i;
        graph[i].push_back({0, 0});
        break;
      }
    }

    //dp[i][j] best amount of points starting from node i with j moves remaining
    std::vector<std::vector<long>> dp(n, std::vector<long>(k+1, -1));

    //Cannot do any points if there are no more moves possible
    for(int i = 0; i < n; i++){
      dp[i][0] = 0;
    }

    for(int j = 1; j <= k; j++){
      for(int i = 0; i < n; i++){

        //If it's the last hole, take the same point
        //as can be done from the beginning with the same number of remaining moves
        if(i == end_v){
          dp[i][j] = dp[0][j];
        }

        //Try to see which one is the best route to take from the current position
        for(edge_t e: graph[i]){
          dp[i][j] = std::max(dp[i][j], dp[e.to][j-1] + e.p);
        }
      }
    }

    //Check if, starting from the hole 0, it's possible to achieve enough points.
    //If so, find how many moves are necessary
    bool found = false;
    for(int i = 0; i <= k; i++){
      if(dp[0][i] >= x){
        std::cout<<i<<"\n";
        found = true;
        break;
      }
    }

    if(!found){
      std::cout<<"Impossible\n";
    }
  }
}
