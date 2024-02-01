#include <iostream>
#include <algorithm>

int main(){
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin>>t;
  while(t--){
    int n;
    std::cin>>n;

    int topple_counter = -1;
    int dominoes = 0;
    for(int i = 0; i < n; i++){
      int h;
      std::cin>>h;

      //If a previous domino can topple this one, or if it's the first
      if(topple_counter != 0){
        topple_counter = std::max(topple_counter, h) - 1;
        dominoes++;
      }
    }

    std::cout<<dominoes<<"\n";
  }
}
