#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>

typedef std::vector<int> VI;
typedef std::vector<VI> VVI;

typedef struct{
  long direct_repair;   //Cost to have the city fixed directly
  long indirect_repair;  //Cost to have the city broken, but fixed by a neighbor
  long maybe_repair;  //Cost if the city *may* be broken, and it is not certain a neighbor fixed it
} city_t;

city_t solve(int current, VVI &graph, VI &costs){
  if(graph[current].size() == 0){  //There are no outgoing cities

    //It's necessary to have costs[i] as the cost of indirect fix, otherwise
    //the city would be considered indirectly fixed for free
    return {costs[current], costs[current], 0};
  }

  long current_direct_rep = costs[current];
  long tot_out_repaired = 0;
  long add_price = INT_MAX;

  for(auto out: graph[current]){  //For each outgoing city
    city_t rep_out = solve(out, graph, costs);

    //Check which is the least expensive way to have the outgoing city already fixed
    long out_repaired = std::min(rep_out.direct_repair, rep_out.indirect_repair);
    tot_out_repaired += out_repaired;

    //Consider the best option between having this node fix the outgoing node and the outgoing node being fixed by someone else
    current_direct_rep += std::min(out_repaired, rep_out.maybe_repair);

    //Check how much would it cost more to be sure that the current node is fixed by an outgoing city
    add_price = std::min(add_price, rep_out.direct_repair - out_repaired);
  }

  //current_direct_rep = fix the current node. Outgoing nodes may be already fixed or not
  //tot_out_repaired + add_price = all the outgoings are fixed, and the additional cost grants that the current is indirectly fixed
  //tot_out_repaired = all the outgoings are fixed, but maybe indirectly. This means that the current one may be broken
  return {current_direct_rep, tot_out_repaired + add_price, tot_out_repaired};
}

int main(){
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin>>t;
  while(t--){
    int n; //n = cities;
    std::cin>>n;

    VVI graph(n);
    for(int i = 0; i < n-1; i++){
      int x, y;
      std::cin>>x>>y;

      graph[x].push_back(y);
    }

    VI costs(n);
    for(int i = 0; i < n; i++){
      int c;
      std::cin>>c;

      costs[i] = c;
    }

    city_t repair = solve(0, graph, costs);
    std::cout<<std::min(repair.direct_repair, repair.indirect_repair)<<"\n";
  }
}
