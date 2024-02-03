#include <iostream>
#include <vector>

#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K> Triangulation;

typedef K::Point_2 P;

typedef int IT;
typedef CGAL::Gmpz ET;

typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

typedef struct{
  int index;
  P point;
  int s;  //supply liters
  int a;  //alcohol %
} warehouse_t;

typedef struct{
  int index;
  P point;
  int d;  //demand liters
  int u;  //limit liters*100
} stadium_t;

typedef struct{
  P point;
  long sq_r;
} contour_t;

long round_down(const CGAL::Quotient<ET> &input_val){
  double num = std::floor(CGAL::to_double(input_val));
  while (num > input_val) num -= 1;
  while (num+1 <= input_val) num += 1;
  return (long) num;
}

int main(){
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin>>t;
  while(t--){
    int n, m, c;  //n = warehouses, m = stadiums, c = contour lines
    std::cin>>n>>m>>c;

    std::vector<P> all_points;

    std::vector<warehouse_t> warehouses(n);
    for(int i = 0; i < n; i++){
      int x, y, s, a;
      std::cin>>x>>y>>s>>a;

      warehouses[i] = {i, P(x, y), s, a};
      all_points.push_back(P(x, y));
    }

    std::vector<stadium_t> stadiums(m);
    for(int i = 0; i < m; i++){
      int x, y, d, u;
      std::cin>>x>>y>>d>>u;

      stadiums[i] = {i, P(x, y), d, 100*u};
      all_points.push_back(P(x, y));
    }

    std::vector<std::vector<long>> revenue(n, std::vector<long>(m));
    for(int i = 0; i < n; i++){
      for(int j = 0; j < m; j++){
        int r;
        std::cin>>r;

        revenue[i][j] = 100*r;  //Multiplied by 100 to have integer revenue. Additional cost will be -1 and not -0.01
      }
    }

    //In the test case 4 description it says that there are at most 100 contours with at least one thing inside
    //Remove unneded contour lines, which are the ones without anything inside
    Triangulation tri;
    tri.insert(all_points.begin(), all_points.end());

    std::vector<contour_t> lines;
    for(int i = 0; i < c; i++){
      long x, y, r;
      std::cin>>x>>y>>r;

      //Keep only the contour lines with a building inside
      auto nearest_building = tri.nearest_vertex(P(x, y));
      long sq_d = CGAL::squared_distance(nearest_building->point(), P(x, y));
      long sq_r = r*r;
      if(sq_d < sq_r){
        lines.push_back({P(x,y), sq_r});
      }
    }

    //Adjust revenues
    for(contour_t con: lines){
      for(auto w: warehouses){
        bool w_inside = CGAL::squared_distance(w.point, con.point) < con.sq_r;
        for(auto s: stadiums){
          bool s_inside = CGAL::squared_distance(s.point, con.point) < con.sq_r;

          //Check if the stadium and the warehouse are on the same side of the contour line
          if(s_inside != w_inside){
            revenue[w.index][s.index] -= 1;
          }
        }
      }
    }

    Program lp (CGAL::SMALLER, true, 0, false, 0);

    //Each variable defines how many beers are sent from w_i to s_j
    //The variable index is w_i*m+s_j

    int counter = 0;

    //Consider the supply of each warehouse
    for(int w = 0; w < n; w++){
      for(int s = 0; s < m; s++){
        int var = w*m+s;  //Liters from w to s

        //Sum all the shipments from warehouse w_i to the different stadiums
        lp.set_a(var, counter, 1);
      }
      lp.set_b(counter, warehouses[w].s);
      counter++;
    }

    //Exact demand
    for(int s = 0; s < m; s++){
      //For each stadium
      //The first two inequalities set the beer amount
      //The third inequlity sets the alcohol limit

      for(int w = 0; w < n; w++){
        int var = w*m+s;  //Liters from w to s

        //Sum of the shipments less than or equal to the maximum
        lp.set_a(var, counter, 1);

        //Sum of the shipments more than or equal to the maximum
        lp.set_a(var, counter+1, -1);

        //Limit the amount of alcohol
        lp.set_a(var, counter+2, warehouses[w].a);
      }
      lp.set_b(counter, stadiums[s].d);
      lp.set_b(counter+1, -stadiums[s].d);
      lp.set_b(counter+2, stadiums[s].u);

      counter += 3;
    }

    //Maximize the profit (minimize the opposite)
    for(int w = 0; w < n; w++){
      for(int s = 0; s < m; s++){
        int var = w*m+s;
        lp.set_c(var, -revenue[w][s]);
      }
    }

    Solution sol = CGAL::solve_linear_program(lp, ET());

    if(sol.is_infeasible()){
      std::cout<<"RIOT!\n";
    }else{
      std::cout<<round_down(-sol.objective_value()/100)<<"\n";
    }
  }
}
