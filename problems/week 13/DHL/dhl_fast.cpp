///3
#include <iostream>
#include <vector>
#include <climits>

int main(){
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin>>t;
  while(t--){
    int n;  //n = parcels
    std::cin>>n;

    //Values are represented from the bottom, but it's necessary to take from the top
    std::vector<int> values_A(n);
    for(int i = 0; i < n; i++){
      int a;
      std::cin>>a;

      values_A[i] = a;
    }

    std::vector<int> values_B(n);
    for(int i = 0; i < n; i++){
      int b;
      std::cin>>b;

      values_B[i] = b;
    }

    std::vector<std::vector<int>> memo(n+1, std::vector<int>(n+1, INT_MAX));

    memo[0][0] = 0;

    for(int i = 1; i <= n; i++){
      for(int j = 1; j <= n; j++){
        memo[i][j] = std::min(memo[i-1][j], memo[i][j-1]);
        memo[i][j] = std::min(memo[i][j], memo[i-1][j-1]);

        memo[i][j] += (values_A[i-1]-1)*(values_B[j-1]-1);
      }
    }

    std::cout<< memo[n][n] <<"\n";
  }
}
