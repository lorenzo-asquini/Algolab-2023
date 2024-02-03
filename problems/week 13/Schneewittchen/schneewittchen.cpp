#include <iostream>
#include <vector>

#include <boost/graph/adjacency_list.hpp>

#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpq.h>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS> graph;
typedef boost::graph_traits<graph>::out_edge_iterator out_edge_it;

typedef int IT;
typedef CGAL::Gmpz ET;

typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

typedef struct{
  int d;  //Dangerous threshold
  std::vector<int> resources;  //Amounts of each type of mineral
} mine_t;

typedef struct{
  int required;
  int supply;
  int price;
} mineral_t;

long floor_to_long(const CGAL::Quotient<ET> &x){
  double a = std::floor(CGAL::to_double(x));
  while (a > x) a -= 1;
  while (a+1 <= x) a += 1;
  return (long)a;
}

//Given a dangerous mine or the entrance, find all the mines downstream until
//the first dangerous mine (that will be included) or the end of the mine in that path

//It's necessary to include also the other dangerous mines firstly encountered in order to
//consider automatically their downstream (after halving)
void find_downstream(int current_mine, int current_dangerous, graph &G, std::vector<std::vector<int>> &mines_downstream, std::vector<mine_t> &mines){

  mines_downstream[current_dangerous].push_back(current_mine);

  //Update the mine whose downstream will be considered
  if(mines[current_mine].d != -1){
    current_dangerous = current_mine;
  }

  out_edge_it out_e, out_eend;
  for(boost::tie(out_e, out_eend) = boost::out_edges(current_mine, G); out_e != out_eend; out_e++){
    find_downstream(boost::target(*out_e, G), current_dangerous, G, mines_downstream, mines);
  }
}

int main(){
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin>>t;
  while(t--){
    int n, m; //n = number of mines, m = different types of minerals
    std::cin>>n>>m;

    std::vector<mine_t> mines(n);

    //If the mine is dangerous, associate its mine index with a dangerous index
    //This allows to have only a few variables in the LP solver
    std::map<int, int> dangerous_index;
    int dangerous_counter = 2;

    for(int i = 0; i < n; i++){
      int d;
      std::cin>>d;
      mines[i].d = d;

      if(d != -1){
        dangerous_index[i] = dangerous_counter++;
      }

      for(int j = 0; j < m; j++){
        int r;
        std::cin>>r;
        mines[i].resources.push_back(r);
      }
    }

    graph G(n);

    for(int i = 0; i < n-1; i++){
      int u, v;
      std::cin>>u>>v;

      boost::add_edge(v, u, G);  //Inverted the mines' order to have railways going away from the entrance
    }

    std::vector<mineral_t> minerals(m);
    for(int i = 0; i < m; i++){
      int c, s, p;
      std::cin>>c>>s>>p;

      minerals[i] = {c, s, p};
    }

    std::vector<std::vector<int>> mines_downstream(n);
    find_downstream(0, 0, G, mines_downstream, mines);

    //There are:
    //- A variable for each dangerous mine (or entrance), for each mineral:
    //--- Each variable contains the amount of that mineral that *goes out* of that dangerous mine
    //--- This means that it may be necessary to multiply it by 2 in order to see what enters the mine
    //--- Variables from the entrance go from 0 to m-1
    //- A variable to represent the bought amount for each mineral (from m to 2m-1)
    Program lp(CGAL::SMALLER, true, 0, false, 0);

    int counter = 0;
    for(int i = 0; i < n; i++){
      if(i == 0 || mines[i].d != -1){  //If the mine is the entrance or dangerous

        for(int j = 0; j < m; j++){  //For each mineral
          int mineral_sum = 0;  //Sum of the current mineral

          //ingoing_materials <= downstream_materials + from_other_dangerous
          for(int k = 0; k < (int) mines_downstream[i].size(); k++){
            int mine_downstream_index = mines_downstream[i][k];

            if(mines[mine_downstream_index].d == -1){  //Sum the minerals downstream from non-dangerous mines
              mineral_sum += mines[mine_downstream_index].resources[j];
            }else{
              lp.set_a(dangerous_index[mine_downstream_index]*m+j, counter, -1);  //Get the resources after halving in downstream dangerous mine
            }
          }

          if(i != 0){
            lp.set_a(dangerous_index[i]*m+j, counter, 2);  //Multiply by 2 because the variable contains the value after halving
          }else{
            lp.set_a(j, counter, 1);  //For the entrance there is no halving
          }
          lp.set_b(counter, mineral_sum);
          counter++;
        }

        if(i != 0){
          //Make sure not to go over the danger threshold
          for(int j = 0; j < m; j++){
            lp.set_a(dangerous_index[i]*m+j, counter, 2);  //Ingoing = 2*outgoing
          }
          lp.set_b(counter, mines[i].d);
          counter++;
        }
      }
    }

    for(int j = 0; j < m; j++){

      lp.set_a(m+j, counter, 1);  //Minerals to purchase
      lp.set_b(counter, minerals[j].supply);
      counter++;

      //purchase + gather >= required
      lp.set_a(m+j, counter, -1);  //Minerals to purchase
      lp.set_a(j, counter, -1);  //Minerals at the entrance
      lp.set_b(counter, -minerals[j].required);
      counter++;

      lp.set_c(m+j, minerals[j].price);  //Minimize the cost
    }

    Solution s = CGAL::solve_linear_program(lp, ET());

    if (s.is_optimal()){
      std::cout<<floor_to_long(s.objective_value())<<std::endl;
    }else{
      std::cout<<"Impossible!"<<std::endl;
    }
  }
}
