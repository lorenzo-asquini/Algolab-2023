#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>

typedef struct{
  int i;
  int j;
  long diff;
} solution_t;

//Due to how the deck is traversed, the current answer has the best indices
void update_best(solution_t &best, int i, int j, long diff){
  if(diff < best.diff){
    best = {i, j, diff};

  }else if(diff == best.diff){

    //Lexicografically smaller
    if(i < best.i){
      best = {i, j, diff};
    }else if(i == best.i && j < best.j){
      best = {i, j, diff};
    }
  }
}

int main(){
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin>>t;
  while(t--){
    int n, k;
    std::cin>>n>>k;

    std::vector<int> values(n);
    for(int i = 0; i < n; i++){
      int v;
      std::cin>>v;

      values[i] = v;
    }

    solution_t best = {-1, -1, INT_MAX};

    int from = 0, to = 0;
    long sum = values[0];

    //Sliding window
    while(from < n){

      if(sum == k){
        update_best(best, from, to, 0);

        if(to < n-1){
          sum += values[++to];
        }else{
          sum -= values[from++];
        }
      }

      if(sum < k && to < n-1){

        //Sum is too low, need to find indices with higher sum
        sum += values[++to];
        update_best(best, from, to, std::abs(k-sum));
      }else{

        //Sum is too high, need to find indices with lower sum
        sum -= values[from++];
        update_best(best, from, to, std::abs(k-sum));
      }
    }

    std::cout<<best.i<<" "<<best.j<<"\n";
  }
}
