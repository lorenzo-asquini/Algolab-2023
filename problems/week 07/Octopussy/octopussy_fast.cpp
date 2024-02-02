#include <iostream>
#include <vector>
#include <algorithm>

int main(){
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin>>t;
  while(t--){
    int n;
    std::cin>>n;

    std::vector<int> times(n);
    for(int i = 0; i < n; i++){
      int exp_t;
      std::cin>>exp_t;

      times[i] = exp_t;
    }

    //Assign to each bomb the minimum time it needs to be defused in order to avoid an explosion
    //Each bomb must be defused before it explodes or 1 second before its parent explodes (minimum of the two)
    for(int j = 0; j <= (n-3)/2; j++){
      times[2*j+1] = std::min(times[j]-1, times[2*j+1]);
      times[2*j+2] = std::min(times[j]-1, times[2*j+2]);
    }

    std::sort(times.begin(), times.end());

    //Check if, by defusing a bomb a second, it's possible to avoid an explosion
    bool explosion = false;
    for(int i = 0; i < n; i++){
      if(i >= times[i]){
        explosion = true;
        break;
      }
    }

    if(explosion){
      std::cout<<"no\n";
    }else{
      std::cout<<"yes\n";
    }
  }
}
