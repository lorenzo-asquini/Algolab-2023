#include <iostream>
#include <vector>
#include <algorithm>

typedef std::vector<int> VI;
typedef std::vector<VI> VVI;

int solve(int i, int j, int k, int m, int turn, VI &coins, VVI &dp){
  if(i == j){  //Only one coin left
    if(turn%m == k){
      dp[i][j] = coins[i];
    }else{
      dp[i][j] = 0;
    }
    return dp[i][j];
  }

  if(dp[i][j] == -1){
    if(turn%m == k){
      //Maximize the points
      dp[i][j] = std::max(solve(i+1, j, k, m, turn+1, coins, dp) + coins[i], solve(i, j-1, k, m, turn+1, coins, dp) + coins[j]);
    }else{
      //Minimize the points for the player k
      dp[i][j] = std::min(solve(i+1, j, k, m, turn+1, coins, dp), solve(i, j-1, k, m, turn+1, coins, dp));
    }
  }

  return dp[i][j];
}

int main(){
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin>>t;
  while(t--){
    int n, m, k;  //n = coins, m = passengers, k = player index
    std::cin>>n>>m>>k;

    VI coins(n);
    for(int i = 0; i < n; i++){
      int x;
      std::cin>>x;

      coins[i] = x;
    }

    //dp[i][j] contains the maximum amount of points the player k can have
    //if the coins from i to j included are considered
    VVI dp(n, VI(n, -1));
    solve(0, n-1, k, m, 0, coins, dp);

    std::cout<<dp[0][n-1]<<"\n";
  }
}
