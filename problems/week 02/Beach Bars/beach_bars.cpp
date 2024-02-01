#include <iostream>
#include <vector>
#include <algorithm>

typedef struct{
  int dist;
  int num;
  std::vector<int> positions;
} solution_t;

void check_best(int dist, int num, int new_pos, solution_t &best){
  //More parasols than the previous best
  if(best.num < num){
    best = {dist, num, {new_pos}};

  //Less maximum distance than the previous best
  }else if(best.num == num && best.dist > dist){
    best = {dist, num, {new_pos}};

  //Same characteristics as the best
  }else if(best.num == num && best.dist == dist){
    best.positions.push_back(new_pos);
  }
}

int main(){
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin>>t;
  while(t--){
    int n;
    std::cin>>n;

    std::vector<int> parasols(n);
    for(int i = 0; i < n; i++){
      int x;
      std::cin>>x;

      parasols[i] = x;
    }

    std::sort(parasols.begin(), parasols.end());

    //First parasol in the range on the left. Vector index
    int first_in = 0;
    //Last parasol in the rang on the right. Vector index
    int last_in = 0;

    //Bar position. Start where there is the first parasol on the left
    int bar_pos = parasols[0];

    //Best position info
    solution_t best = {0, 1, {parasols[0]}};

    //Current info
    int dist = 0;
    int num = 1;

    while(bar_pos < parasols[n-1]){

      //While new parasols can be included on the right
      while(last_in+1 < n && parasols[last_in+1] <= bar_pos+100){
        last_in++;
        num++;
      }

      //While parasols needs to be excluded on the left
      while(parasols[first_in] < bar_pos-100){
        first_in++;
        num--;
      }

      dist = std::max(std::abs(bar_pos-parasols[first_in]), std::abs(bar_pos-parasols[last_in]));
      check_best(dist, num, bar_pos, best);

      bar_pos++;
    }

    std::cout<<best.num<<" "<<best.dist<<"\n";

    //Already sorted
    for(int pos: best.positions){
      std::cout<<pos<<" ";
    }

    std::cout<<"\n";
  }
}
