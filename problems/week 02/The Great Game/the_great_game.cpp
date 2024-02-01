#include <iostream>
#include <vector>
#include <climits>

typedef std::vector<int> VI;
typedef std::vector<VI> VVI;

int solve(int current, int n, VVI &transitions, VI &best, VI &worst, bool is_best){

  if(is_best){

    if(best[current] == INT_MAX){
      for(int out: transitions[current]){
        //Consider that, from the current position, the other player will do the worst move from the landing position
        best[current] = std::min(best[current], solve(out, n, transitions, best, worst, false)+1);
      }
    }
    return best[current];
  }else{

    if(worst[current] == -1){
      for(int out: transitions[current]){
        //Consider that, from the current position, the other player will do the best move from the landing position
        worst[current] = std::max(worst[current], solve(out, n, transitions, best, worst, true)+1);
      }
    }
    return worst[current];
  }
}

int main(){
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin>>t;
  while(t--){
    int n, m; //n = positions, m = transitions
    std::cin>>n>>m;

    int r, b; //Red and black meeples starting position
    std::cin>>r>>b;

    //Positions go from 1 to n included
    VVI transitions(n+1);
    for(int i = 0; i < m; i++){
      int u, v;
      std::cin>>u>>v;

      transitions[u].push_back(v);
    }

    //Best and worst number of steps to reach the end starting from position i
    VI best(n+1, INT_MAX);
    VI worst(n+1, -1);

    best[n] = 0;
    worst[n] = 0;

    solve(r, n, transitions, best, worst, true);
    solve(b, n, transitions, best, worst, true);

    std::cout<<best[r]<<" "<<best[b]<<"\n";

    if(best[r] < best[b] || (best[r] == best[b] && best[r] % 2 == 1)){
      std::cout<<"0\n";  //Red arrives first
    }else{
      std::cout<<"1\n";  //Black arrives first
    }
  }
}
