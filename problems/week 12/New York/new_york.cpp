#include <iostream>
#include <vector>
#include <deque>
#include <set>

typedef std::vector<int> VI;
typedef std::vector<VI> VVI;

void solve(int next, std::deque<int> &path, std::multiset<int> &path_temps, VI &temps, VVI &graph, VI &starts, int m, int k){

  int removed = -1;

  //The path may be too long at this point
  if((int)path.size() == m){
    removed = path.front();
    path.pop_front();

    //Cannot use erase(value) because it deletes all instances
    path_temps.erase(path_temps.find(temps[removed]));
  }

  path.push_back(next);
  path_temps.insert(temps[next]);

  //Check if the path is valid
  if((int)path.size() == m){
    //The first element of a multiset is the smallest, the last the largest
    int max = *path_temps.rbegin();
    int min = *path_temps.begin();

    if(max-min <= k){
      starts.push_back(path.front());
    }
  }

  //Explore children recursively
  for(int p: graph[next]){
    solve(p, path, path_temps, temps, graph, starts, m, k);
  }

  //At this point, it's necessary to restore the original state of the path
  //This allows to not need a copy for each recursive call
  path_temps.erase(path_temps.find(temps[next]));
  path.pop_back();

  //Restore first element of path.
  if(removed != -1){
    path_temps.insert(temps[removed]);
    path.push_front(removed);
  }
}

int main(){
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin>>t;
  while(t--){
    int n, m, k; //n = points, m = length, k = max risk
    std::cin>>n>>m>>k;

    VI temps(n);
    for(int i = 0; i < n; i++){
      int h;
      std::cin>>h;

      temps[i] = h;
    }

    VVI graph(n);
    for(int i = 0; i < n-1; i++){
      int u, v;
      std::cin>>u>>v;

      graph[u].push_back(v);
    }

    VI starts;
    std::deque<int> path;
    std::multiset<int> path_temps;

    solve(0, path, path_temps, temps, graph, starts, m, k);

    //Reorder the start positions
    std::set<int> ordered_starts;
    ordered_starts.insert(starts.begin(), starts.end());

    if(ordered_starts.size() == 0){
      std::cout<<"Abort mission\n";
    }else{
      for(int s: ordered_starts){
        std::cout<<s<<" ";
      }
      std::cout<<"\n";
    }
  }
}
