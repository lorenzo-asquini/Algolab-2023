#include <iostream>
#include <vector>
#include <algorithm>

typedef struct{
  int to;
  int l;
} edge_t;

typedef struct{
  long galleons;  //Number of galleons collected
  int chambers;  //Chambers traversed starting from the current chamber (current chamber included)
  long return_time;  //Time needed to traverse all nodes starting from the current node
} path_t;

struct compPaths{
  //It's better to take p1 first and remove galleons from p2 according to the time needed to traverse p1
  //Or it's better to take p2 first
  bool operator()(path_t &p1, path_t &p2){
    return p2.chambers * p1.return_time < p1.chambers * p2.return_time;
  }
};

path_t solve(int current, std::vector<std::vector<edge_t>> &graph, std::vector<int> &galleons){
  if(graph[current].size() == 0){  //No outgoing edges
    return {galleons[current], 1, 0};
  }

  std::vector<path_t> paths;
  for(auto out: graph[current]){
    path_t possible_path = solve(out.to, graph, galleons);

    //Add to the path the time needed to enter that path
    //Remove the disappeared coins from travelling through the entering tunnel to the path
    paths.push_back(
      {possible_path.galleons - out.l * possible_path.chambers,
      possible_path.chambers,
      possible_path.return_time + out.l*2});
  }

  std::sort(paths.begin(), paths.end(), compPaths());

  long tot_galleons = galleons[current];
  int tot_chambers = 0;
  long tot_return_time = 0;

  for(auto p: paths){
    //Remove the galleons lost while travelling the other paths that start from the current node
    tot_galleons += (p.galleons - p.chambers * tot_return_time);

    tot_return_time += p.return_time;
    tot_chambers += p.chambers;
  }

  return {tot_galleons, tot_chambers+1, tot_return_time};
}

int main(){
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin>>t;
  while(t--){
    int n;
    std::cin>>n;

    std::vector<int> galleons(n+1);
    for(int i = 0; i < n; i++){
      int g;
      std::cin>>g;

      galleons[i] = g;
    }

    std::vector<std::vector<edge_t>> graph(n+1);
    for(int i = 0; i < n; i++){
      int u, v, l;  //From u to v, with length l
      std::cin>>u>>v>>l;

      graph[u].push_back({v, l});
    }

    std::cout<<solve(0, graph, galleons).galleons<<"\n";
  }
}
