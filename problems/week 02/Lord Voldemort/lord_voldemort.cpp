#include <iostream>
#include <vector>
#include <algorithm>

typedef std::vector<int> VI;
typedef std::vector<VI> VVI;

int main(){
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin>>t;
  while(t--){
    int n, m, k;
    std::cin>>n>>m>>k;  //n = objects, m = members, k = power

    VI evils(n);
    for(int i = 0; i < n; i++){
      int v;
      std::cin>>v;

      evils[i] = v;
    }

    //Sliding window to find all possible windows
    //windows[i] = j means that from point i there is a window until j included

    int low = 0;
    int high = 0;
    int sum = evils[0];
    VI windows(n, -1);

    while(low < n){
      if(sum == k){
        windows[low] = high;

        if(high < n-1){
          sum += evils[++high];
        }else{
          sum -= evils[low++];
        }
        continue;
      }

      if(sum < k && high < n-1){
        sum += evils[++high];
      }else{
        sum -= evils[low++];
      }
    }

    //dp[i][j] where i is the first new object encountered and j is the number of remaining members
    VVI dp(n+1, VI(m+1, -1));

    for(int i = 0; i < n; i++){
      dp[i][0] = 0;  //No members remaining
    }

    for(int i = 0; i <= m; i++){
      dp[n][i] = 0; //All objects destroyed
    }

    for(int i = n-1; i >= 0; i--){
      for(int j = 1; j <= m; j++){

        if(windows[i] != -1){  //If there is a window starting at i
          int w_end = windows[i];
          int objects = w_end - i + 1;

          //Skip the object or destroy the window
          dp[i][j] = std::max(dp[i+1][j], dp[w_end+1][j-1] + objects);
        }else{
          //No window starting at i, skip the current object
          dp[i][j] = dp[i+1][j];
        }
      }
    }

    //Check if each member destroyed at least one object
    bool change = true;
    for(int i = 1; i <= m; i++){
      if(dp[0][i-1] == dp[0][i]){
        change = false;
      }
    }

    if(!change){
      std::cout<<"fail\n";
    }else{
      std::cout<<dp[0][m]<<"\n";
    }
  }
}
