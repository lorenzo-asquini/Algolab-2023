#include <iostream>
#include <vector>
#include <climits>
#include <set>
#include <cmath>

typedef std::vector<int> VI;
typedef std::vector<VI> VVI;
typedef std::vector<VVI> VVVI;
typedef std::vector<VVVI> VVVVI;

typedef struct {
  int f1;  //Most recent
  int f2;
  int f3;
} fighters_queue_t;

//Num of distinct fighters considering the last m fighters
int num_distinct(fighters_queue_t &q, int m){

    //Very fast way to find how many distinct fighters people remember
    //Remove 1 to not count the absence of a fighter
    if (m == 2){
      return std::set<int>({0, q.f1, q.f2}).size() - 1;
    }else{
      return std::set<int>({0, q.f1, q.f2, q.f3}).size() - 1;
    }
}

int solve(VVVVI &dp, int n, int m, VI &fighters, fighters_queue_t &north, fighters_queue_t &south, int diff){

    if (std::abs(diff) >= 12){  //2^12 = 4096 > 3000, avoid negative excitement
      return INT_MIN;
    }

    if (n == -1){  //No more fighters remaining
      return 0;
    }

    //Encode last two fighters
    int encode_north = 5 * north.f1 + north.f2;
    int encode_south = 5 * south.f1 + south.f2;

    //Difference can be negative
    int shifted_diff = diff + 12;

    if (dp[n][encode_north][encode_south][shifted_diff] != -1){
      return dp[n][encode_north][encode_south][shifted_diff];
    }

    int next_f = fighters[n];

    //Create the two new possible queues of last fighters depending on which entrace is chosen
    fighters_queue_t new_north = {next_f, north.f1, north.f2};
    fighters_queue_t new_south = {next_f, south.f1, south.f2};

    //Excitement in this round
    int exc_if_north = num_distinct(new_north, m)*1000 - int(std::pow(2, std::abs(diff+1)));
    int exc_if_south = num_distinct(new_south, m)*1000 - int(std::pow(2, std::abs(diff-1)));

    //Find best value depending on which entrance is the chosen one
    int best = INT_MIN;
    if (exc_if_north >= 0){  //Excitement in each round must be positive
      best = exc_if_north + solve(dp, n-1, m, fighters, new_north, south, diff+1);
    }

    if (exc_if_south >= 0){
      best = std::max(best, exc_if_south + solve(dp, n-1, m, fighters, north, new_south, diff-1));
    }

    dp[n][encode_north][encode_south][shifted_diff] = best;
    return best;
}

int main(){
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin>>t;
  while(t--){
    int n, k, m;  //n = fighters, k = types of fighters, m = remember
    std::cin>>n>>k>>m;

    //Store fighters backwards in order to start from index 0 when going through the queue
    VI fighters(n);
    for(int i = n-1; i >= 0; i--){
      int x;
      std::cin>>x;

      //Add 1 to the fighter type to use 0 as "no fighter"
      fighters[i] = x+1;
    }

    //dp[a][b][c][d] where:
    //* a denotes the current fighter being processed
    //* b encodes the last two fighters coming from the north
    //* c encodes the last two fighters coming from the south
    //* d denotes the difference in entrance between the north and the south
    ////Size choosen for the worst case scenario
    VVVVI dp(n, VVVI(25, VVI(25, VI(25, -1))));

    //It's necessary to consider that there can be 5 different fighters (no fighter + max_k)
    //At maximum the spectators can remember 3 fighters. Considering that the third one is
    //the one newly inserted, it's possible to just store the previous two by encoding them using x*5+y

    //The excitement cannot be negative. Considering that the maximum excitement for a turn can be
    //only 3000 (last 3 * 1000), |p-q| < 12 to have a positive number. For this reason it's possible
    //to store the difference using only 25 cells (with a up-shift for negative numbers)

    //Fighter with value 0 means no fighter
    fighters_queue_t empty = {0, 0, 0};
    std::cout<< solve(dp, n-1, m, fighters, empty, empty, 0) <<"\n";
  }
}
