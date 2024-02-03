#include <iostream>
#include <vector>

typedef std::vector<int> VI;
typedef std::vector<VI> VVI;

typedef struct{
  int other;
  int rats;
} street_t;

int main(){
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin>>t;
  while(t--){
    int n, m;  //n = squares, m = streets
    std::cin>>n>>m;

    std::vector<std::vector<street_t>> graph(n);
    std::vector<std::vector<street_t>> inv_graph(n);
    for(int i = 0; i < m; i++){
      int u, v, f;
      std::cin>>u>>v>>f;

      graph[u].push_back({v, f});
      inv_graph[v].push_back({u, f});
    }

    //dp[i][j] contains the highest amount of rats that can be taken going from 0 to i
    //and from j to 0. The final answer is dp[n-1][n-1]

    //This means I have to try all the possible combinations that reach n-1 going through i
    //And return back though j
    VVI dp(n, VI(n, 0));

    //It's necessary to populate the starting point because non-populated cells will be skipped because unreachable

    //Populate all the squares that can be reached from 0 while going forward
    for(street_t s: graph[0]){
      dp[s.other][0] = s.rats;
    }

    //Populate all the squares that can reach 0 while going backward
    for(street_t s: inv_graph[0]){
      dp[0][s.other] = s.rats;
    }

    //Try all the possible cells
    for(int i = 0; i < n; i++){
      for(int j = 0; j < n; j++){

        //Consider only the cells that were populated, that means that they are
        //reachable (going forward or backward)
        if(dp[i][j] != 0){

          //Try all the landing positions going forward from i
          for(auto out_i: graph[i]){

            //Consider only the landing positions with a square ID higher than the starting
            //point for the backward path. This avoids using the same square both ways
            if(out_i.other > j || out_i.other == n-1){
              dp[out_i.other][j] = std::max(dp[out_i.other][j], dp[i][j] + out_i.rats);
            }
          }

          //Try all the landing positions going backward from j
          for(auto in_j: inv_graph[j]){

            //Consider only the landing positions with a square ID higher than the starting
            //point for the forward path. This avoids using the same square both ways
            if(in_j.other > i || in_j.other == n-1){
              dp[i][in_j.other] = std::max(dp[i][in_j.other], dp[i][j] + in_j.rats);
            }
          }
        }
      }
    }

    std::cout<<dp[n-1][n-1]<<"\n";
  }
}
