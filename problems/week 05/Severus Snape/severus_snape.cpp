#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>

typedef std::vector<long> VL;
typedef std::vector<VL> VVL;
typedef std::vector<VVL> VVVL;

typedef struct{
  long p;
  long h;
} potA;

int main(){
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin>>t;
  while(t--){
    int n, m; //n = pot A, m = pot B
    std::cin>>n>>m;

    long a, b;  //a = lose wit with A, b = lose power with B
    std::cin>>a>>b;

    long P, H, W;  //Target values
    std::cin>>P>>H>>W;

    std::vector<potA> potionsA(n);
    for(int i = 0; i < n; i++){
      int p, h;
      std::cin>>p>>h;

      potionsA[i] = {p, h};
    }

    VL potionsB(m);
    for(int i = 0; i < m; i++){
      int w;
      std::cin>>w;

      potionsB[i] = w;
    }

    //dp[j][i][h]. Take j potions out of the first i potions that have a happines of at least h
    VVVL dp(n+1, VVL(n+1, VL(H+1, LONG_MIN)));

    //Base case, take only one potion
    for(int h = 0; h <= H; h++){
      for(int i = 1; i < n+1; i++){  //Out of the first i potions, take only 1
        dp[1][i][h] = dp[1][i-1][h];  //Take the best single potion when only i-1 potions were considered

        //Check if the new potion is better than a previous one to reach an happines of h
        if(potionsA[i-1].h >= h && potionsA[i-1].p > dp[1][i][h]){
          dp[1][i][h] = potionsA[i-1].p;
        }
      }
    }

    for(int j = 2; j < n+1; j++){  //Take j potions
      for(int h = 0; h <= H; h++){  //With happines at least h
        for(int i = j; i < n+1; i++){  //Out of the first i potions

          //Previous happiness because, by taking the current potion, it arrives at h
          int prev_h = std::max(0L, h - potionsA[i-1].h);

          //Take the current potion instead of the one that was taken previously or not
          dp[j][i][h] = std::max(dp[j][i-1][h], dp[j-1][i-1][prev_h] + potionsA[i-1].p);
        }
      }
    }

    //Greedy approach for the B potions
    std::sort(potionsB.begin(), potionsB.end(), std::greater<long>());

    int num_potions_A = -1;
    int num_potions_B = 0;

    long sum_wit = 0;

    for(; num_potions_B < m; num_potions_B++){
      sum_wit += potionsB[num_potions_B];

      if(sum_wit >= W){
        for(int pA = 1; pA < n+1; pA++){
          long power = dp[pA][n][H];

          //Check that the wit remains high enough
          if(sum_wit-a*pA < W){
            break;
          }

          if(power >= P+b*(num_potions_B+1)){
            num_potions_A = pA;
            break;
          }
        }
      }

      if(num_potions_A != -1){
        break;
      }
    }

    if(num_potions_A == -1){
      std::cout<<"-1\n";
    }else{
      //+1 because num_potions_B is an index
      std::cout<<num_potions_A + num_potions_B + 1<<"\n";
    }
  }
}
