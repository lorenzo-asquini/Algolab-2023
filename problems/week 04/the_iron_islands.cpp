#include <iostream>
#include <vector>
#include <algorithm>

typedef struct{
  int w;  //Waterway index
  int par_sum;  //Partial sum of men
  int nr_islands;  //Number of islands considered for the partial sum
} path_t;

struct orderPaths{
  bool operator()(path_t &p1, path_t &p2){
   if(p1.par_sum == p2.par_sum){
     return p1.nr_islands < p2.nr_islands;
   }
   return p1.par_sum < p2.par_sum;
  }
};

int main(){
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin>>t;
  while(t--){
    int n, k, b;  //n = islands, k = men, b = waterways
    std::cin>>n>>k>>b;

    std::vector<int> required(n);
    for(int i = 0; i < n; i++){
      int c;
      std::cin>>c;

      required[i] = c;
    }

    std::vector<std::vector<int>> waterways(b);
    std::vector<path_t> paths;
    for(int w = 0; w < b; w++){
      int l;
      std::cin>>l;

      int par_sum = 0;
      for(int i = 0; i < l; i++){
        int r;
        std::cin>>r;

        waterways[w].push_back(r);
        par_sum += required[r];

        paths.push_back({w, par_sum, i+1});
      }
    }

    //The best path considers only one wateray. Use the sliding window technique
    int max_islands = 0;
    for(int w = 0; w < b; w++){
      int low = 0;
      int high = 0;
      int sum = required[waterways[w][0]];

      while(low < (int)waterways[w].size()){

        if(sum == k){
          max_islands = std::max(max_islands, high-low+1);

          if(high < (int)waterways[w].size()-1){
            sum += required[waterways[w][++high]];
          }else{
            sum -= required[waterways[w][low++]];
          }
          continue;
        }

        if(sum < k && high < (int)waterways[w].size()-1){
          sum += required[waterways[w][++high]];
        }else{
          sum -= required[waterways[w][low++]];
        }
      }
    }

    //The best path uses two waterways passing though the center. Use the two sum technique
    std::sort(paths.begin(), paths.end(), orderPaths());
    int low = 0;
    int high = paths.size()-1;

    while(low <= high){
      //Remove the men required to conquer island 0 because they are considered in both partial sums
      int sum = paths[low].par_sum + paths[high].par_sum - required[0];
      if(sum == k){
        if(paths[low].w != paths[high].w){
          //Remove the island 0 because it is considered in both island counts
          max_islands = std::max(max_islands, paths[low].nr_islands + paths[high].nr_islands - 1);
        }

        low++;
        continue;
      }

      if(sum < k){
        low++;
      }else{
        high--;
      }
    }

    std::cout<<max_islands<<"\n";
  }
}
