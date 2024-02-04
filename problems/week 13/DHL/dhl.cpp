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
    std::vector<int> values_A(n+1);
    for(int i = 0; i < n; i++){
      int a;
      std::cin>>a;

      values_A[i] = a;
    }
    values_A[n] = 0;  //Easier calculations later

    std::vector<int> values_B(n+1);
    for(int i = 0; i < n; i++){
      int b;
      std::cin>>b;

      values_B[i] = b;
    }
    values_B[n] = 0;

    //In order to avoid doing sums for every iteration, the sums are already stored in the array
    //To get the boxes from i to j included, with i on the top of j (so i >= j), it's necessary to take
    //values[j] - values[i+1]
    for(int i = n-1; i >= 0; i--){
      values_A[i] = values_A[i] + values_A[i+1];
      values_B[i] = values_B[i] + values_B[i+1];
    }

    //dp[i][j] contains the minimum value when there are still i boxes A and j boxes B
    //This means that the answer is dp[n][n], that is the case when all boxes are still present
    std::vector<std::vector<long>> dp(n+1, std::vector<long>(n+1, LONG_MAX));

    //Initialise the DP table considering that it's necessary to get all the last boxes at the same time
    for(int i = n-1; i >= 1; i--){
      //1 box A (boxes with index from 0 to 0), i boxes B (boxes with index from 0 to i-1)
      //There can be a maximum of n-1 box B remaining because, to reach the case where only one box
      //Remains in A, it was necessary to take at least one box from B
      //Remember that 0 is the last box at the bottom
      long cost_A = values_A[0]-values_A[1]-1;
      long cost_B = values_B[0]-values_B[i]-i;

      dp[1][i] = cost_A * cost_B;

      //1 box B (boxes with index from 0 to 0), i boxes A (boxes with index from 0 to i-1)
      //There can be a maximum of n-1 box A remaining because, to reach the case where only one box
      //Remains in B, it was necessary to take at least one box from A
      //Remember that 0 is the last box at the bottom
      cost_A = values_A[0]-values_A[i]-i;
      cost_B = values_B[0]-values_B[1]-1;

      //Using min to avoid problems in the case dp[1][1] where both cases want to set a value
      dp[i][1] = std::min(dp[i][1], cost_A * cost_B);
    }

    //No boxes remaining to take, no value
    dp[0][0] = 0;

    for(int rem_A = 2; rem_A <= n; rem_A++){
      for(int rem_B = 2; rem_B <= n; rem_B++){
        int take_A, take_B;

        //The best strategy considers only taking 1 box from a pile and any amount from the other
        take_A = 1;
        for(int take_B = 1; take_B < rem_B; take_B++){
          long cost_A = values_A[rem_A-take_A]-values_A[rem_A]-take_A;
          long cost_B = values_B[rem_B-take_B]-values_B[rem_B]-take_B;

          dp[rem_A][rem_B] = std::min(dp[rem_A][rem_B], cost_A*cost_B + dp[rem_A-take_A][rem_B-take_B]);
        }

        take_B = 1;
        for(int take_A = 1; take_A < rem_A; take_A++){
          long cost_A = values_A[rem_A-take_A]-values_A[rem_A]-take_A;
          long cost_B = values_B[rem_B-take_B]-values_B[rem_B]-take_B;

          dp[rem_A][rem_B] = std::min(dp[rem_A][rem_B], cost_A*cost_B + dp[rem_A-take_A][rem_B-take_B]);
        }
      }
    }

    std::cout<<dp[n][n]<<"\n";
  }
}
