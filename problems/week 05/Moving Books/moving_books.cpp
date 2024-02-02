#include <iostream>
#include <vector>
#include <algorithm>
#include <map>

int main(){
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin>>t;
  while(t--){
    int n, m; //n = friends, m = boxes
    std::cin>>n>>m;

    std::vector<int> strengths(n);
    for(int i = 0; i < n; i++){
      int s;
      std::cin>>s;

      strengths[i] = s;
    }

    std::sort(strengths.begin(), strengths.end(), std::greater<int>());

    //Store how many boxes of each weight there are
    std::map<int, int> weights;
    for(int i = 0; i < m; i++){
      int w;
      std::cin>>w;

      weights[w]++;
    }

    //If the strongest friend cannot lift the heaviest box, it's impossible
    if(strengths[0] < weights.begin()->first){
      std::cout<<"impossible\n";
    }

    int minutes = 0;
    bool impossible = false;
    while(true){
      minutes += 2;  //Time to go down the stairs

      //Try to find a matching box for each friend
      for(int i = 0; i < n; i++){
        int s = strengths[i];

        //Search for the first valid weight
        auto equal_greater_it = weights.lower_bound(s);
        while(equal_greater_it != weights.begin() && (equal_greater_it == weights.end() || equal_greater_it->first > s)){
          equal_greater_it--;
        }

        //No valid weight found
        if(equal_greater_it->first > s){
          if(i == 0){  //Impossible if there is not a weight for the strongest
            impossible = true;
          }
          break;  //No need to check also for weaker friends
        }

        (equal_greater_it->second)--;

        if(equal_greater_it->second == 0){
          weights.erase(equal_greater_it);
        }

        if(weights.empty()){
          break;
        }
      }

      if(impossible || weights.empty()){
        break;
      }else{
        minutes += 1;  //Go up the stairs because there are more boxes
      }
    }

    if(impossible){
      std::cout<<"impossible\n";
    }else{
      std::cout<<minutes<<"\n";
    }
  }
}
