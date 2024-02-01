#include <iostream>
#include <vector>
#include <algorithm>

typedef std::vector<int> VI;
typedef std::vector<VI> VVI;

int solve(bool is_me, int i, int j, VVI &best, VI &values){
  if(i == j){  //Only one coin remaining
    if(is_me){
      best[i][j] = values[i];
      return best[i][j];
    }else{
      return 0;
    }
  }

  if(best[i][j] == -1){
    if(is_me){

      //Take a coin and remove it from the pile
      best[i][j] = std::max(solve(false, i+1, j, best, values) + values[i], solve(false, i, j-1, best, values) + values[j]);
    }else{

      //Just remove the coin
      best[i][j] = std::min(solve(true, i+1, j, best, values), solve(true, i, j-1, best, values));
    }
  }

  return best[i][j];
}

int main(){
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin>>t;
  while(t--){
    int n;
    std::cin>>n;

    std::vector<int> values(n);
    for(int i = 0; i < n; i++){
      int v;
      std::cin>>v;

      values[i] = v;
    }

    //best[i][j] contains the best winning value considering the coins between i and j included
    VVI best(n, VI(n, -1));

    solve(true, 0, n-1, best, values);

    std::cout<<best[0][n-1]<<"\n";
  }
}
