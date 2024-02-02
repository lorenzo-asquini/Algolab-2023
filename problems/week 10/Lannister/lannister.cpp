#include <iostream>
#include <vector>

#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

typedef long IT;
typedef CGAL::Gmpz ET;

typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;

typedef K::Point_2 P;

long ceil_to_long(const CGAL::Quotient<CGAL::Gmpz> & x){
  double a = std::ceil(CGAL::to_double(x));
  while (a < x) a += 1;
  while (a-1 >= x) a -= 1;
  return a;
}

int main(){
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin>>t;
  while(t--){
    int n, m; long s;  //n = nobles, m = commons, s = max sum of sewage pipes
    std::cin>>n>>m>>s;

    //Water canal -> Non-vertical: a_w * x + b_w * y + c_w = 0 (b_w != 0. Possible to just set b_w to a value different than 0)
    //Sewage canal -> Non-horizontal: a_s * x + b_s * y + c_s = 0 (a_s != 0. Possible to just set a_s to a value different than 0)
    //Water canal is perpendicular to the sewage canal
    //Water pipes -> Only vertical
    //Sewage pipes -> Only horizontal

    std::vector<P> nobles(n);
    long sum_nobles_x = 0;
    long sum_nobles_y = 0;
    for(int i = 0; i < n; i++){
      int x, y;
      std::cin>>x>>y;

      nobles[i] = P(x, y);
      sum_nobles_x += x;
      sum_nobles_y += y;
    }

    std::vector<P> commons(m);
    long sum_commons_x = 0;
    long sum_commons_y = 0;
    for(int i = 0; i < m; i++){
      int x, y;
      std::cin>>x>>y;

      commons[i] = P(x, y);
      sum_commons_x += x;
      sum_commons_y += y;
    }

    Program lp (CGAL::SMALLER, false, 0, false, 0);

    //Sewage canal: ax+by+c=0. Considering a != 0, then a = 1
    //Constants to LP variables
    const int b_s = 0;
    const int c_s = 1;

    int counter = 0;

    //DIVISION COMMONS AND NOBLES
    //Cersei -> Nobles on the left of sewage canal, commons on its right -> If not, "Yuck!"

    //P(x, y) on the right if by+c <= -x. On the left if x+by+c >= 0 (-> -by-c <= x)
    for(int i = 0; i < n; i++){
      P h = nobles[i];

      lp.set_a(b_s, counter, h.y());
      lp.set_a(c_s, counter, 1);
      lp.set_b(counter, -h.x());

      counter++;
    }

    for(int i = 0; i < m; i++){
      P h = commons[i];

      lp.set_a(b_s, counter, -h.y());
      lp.set_a(c_s, counter, -1);
      lp.set_b(counter, h.x());

      counter++;
    }

    Solution sol = CGAL::solve_linear_program(lp, ET());

    if(sol.is_infeasible()){
      std::cout<<"Yuck!\n";
      continue;
    }

    //SUM OF SEWAGE PIPES LESS THAN s
    //Tywin -> Sum of lengths of sewage pipes <= s -> If not, "Bankrupt!"

    if(s != -1){
      //All sewage pipes are horizontal, meaning that their length is:
      //For nobles on the left: - b_s * y_house - c - x_house [with a_s = 1]
      //For commons on the right: x_house + b_s * y_house + c [with a_s = 1]
      //Meaning that all the nobles pipes are: - b * sum(y_house) - n*c - sum(x_house)
      //And that all the commons pipes are: b * sum(y_house) + m*c + sum(x_house)

      lp.set_a(b_s, counter, sum_commons_y - sum_nobles_y);
      lp.set_a(c_s, counter, m-n);
      lp.set_b(counter, s + sum_nobles_x - sum_commons_x);
      counter++;

      Solution sol = CGAL::solve_linear_program(lp, ET());

      if(sol.is_infeasible()){
        std::cout<<"Bankrupt!\n";
        continue;
      }
    }

    //MINIMIZE LENGTH WATER PIPES
    //Water canal: ax+by+c=0. Considering b != 0, then b = 1
    //Constants to LP variables
    const int a_w = 2;
    const int c_w = 3;

    //Canals are perpendicular. b_s = -a_w -> b_s + a_w <= 0 && -b_s - a_w <= 0
    lp.set_a(b_s, counter, 1);
    lp.set_a(a_w, counter, 1);
    counter++;

    lp.set_a(b_s, counter, -1);
    lp.set_a(a_w, counter, -1);
    counter++;

    //Create a variable that limits the length of the water pipes in order to minimize it
    const int L = 4;
    lp.set_l(L, true, 0);

    //Vertical connection, so the the connection point is y = -a_w * x_house - c_w.
    //The distance is L = |y_house - y| = |y_house + a_w * x_house + c|
    //It's possible to use L >= y_house - y and L >= -y_house + y,
    //because one will be the correct one, and one will be negative (ignored)

    //Nobles
    for(int i = 0; i < n; i++){
      P h = nobles[i];

      //L >= y_house + a_w * x_house + c <-> a_w * x_house + c_w - L<= - y_house
      lp.set_a(a_w, counter, h.x());
      lp.set_a(c_w, counter, 1);
      lp.set_a(L, counter, -1);
      lp.set_b(counter, -h.y());
      counter++;

      //L >= -y_house - a_w * x_house - c_w <-> - a_w * x_house - c_w - L <= y_house
      lp.set_a(a_w, counter, -h.x());
      lp.set_a(c_w, counter, -1);
      lp.set_a(L, counter, -1);
      lp.set_b(counter, h.y());
      counter++;
    }

    //Commons
    for(int i = 0; i < m; i++){
      P h = commons[i];

      //L >= y_house + a_w * x_house + c <-> a_w * x_house + c_w - L<= - y_house
      lp.set_a(a_w, counter, h.x());
      lp.set_a(c_w, counter, 1);
      lp.set_a(L, counter, -1);
      lp.set_b(counter, -h.y());
      counter++;

      //L >= -y_house - a_w * x_house - c_w <-> - a_w * x_house - c_w - L <= y_house
      lp.set_a(a_w, counter, -h.x());
      lp.set_a(c_w, counter, -1);
      lp.set_a(L, counter, -1);
      lp.set_b(counter, h.y());
      counter++;
    }

    //Minimize the limiting length
    lp.set_c(L, 1);

    sol = CGAL::solve_linear_program(lp, ET());
    std::cout<<ceil_to_long(sol.objective_value())<<"\n";
  }
}
