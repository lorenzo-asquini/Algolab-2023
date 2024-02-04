#include <iostream>
#include <vector>

#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpq.h>

typedef CGAL::Gmpq IT;
typedef CGAL::Gmpq ET;

typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

struct nail{
  int x;
  int y;
};

//The answer is a quotient of two quotients
long floor_to_long(const CGAL::Quotient<ET> &x){
  double a = std::floor(CGAL::to_double(x));
  while (a > x) a -= 1;
  while (a + 1 <= x) a += 1;
  return a;
}

int main(){
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin>>t;

  while(t--){
    int n, m, h, w; //n = new posters, m = old posters, h = base height, w = base width
    std::cin>>n>>m>>h>>w;

    std::vector<struct nail> new_posters(n);
    //Save new posters
    for(int i = 0; i < n; i++){
      int x, y;
      std::cin>>x>>y;

      new_posters[i] = {x, y};
    }

    std::vector<struct nail> old_posters(m);
    //Save old posters
    for(int i = 0; i < m; i++){
      int x, y;
      std::cin>>x>>y;

      old_posters[i] = {x, y};
    }

    //Magnifications cannot be lower than 1
    Program lp (CGAL::SMALLER, true, 1, false, 0);

    int in_number = 0;  //Number of the current inequality
    for(int i = 0; i < n; i++){
      for(int j = i+1; j < n; j++){

        //The limits for new posters are (if x1 < x2):

        //x1 + a1*w/2 < x2 - a2*w/2
        //w/2 * (a1+a2) < x2 - x1
        //a1+a2 < 2*(x2-x1)/w

        //To avoid checking for x1 < x2, it's possible to take the absolute value
        //The same thing applies for the y axis

        int x_dist_2 = 2*(std::abs(new_posters[j].x - new_posters[i].x));
        int y_dist_2 = 2*(std::abs(new_posters[j].y - new_posters[i].y));

        lp.set_a(i, in_number,  1);
        lp.set_a(j, in_number,  1);

        //While growing, the posters may overlap first on the x axis or the y axis
        //For this reason, it's necessary to limit the growth to the axis that will be limited first
        //It's necessary to pick the biggest between 2*|x2-x1|/w and 2*|y2-y1|/h
        //(Do not pick the minimum. Think about the case where x2 = x1, that would lead to 0 as the limit)
        lp.set_b(in_number, std::max(IT(y_dist_2, h), IT(x_dist_2, w)));
        in_number++;
      }
    }

    //For each new poster, limit the growth to the closest old poster
    for(int i = 0; i < n; i++){
      //x1 + a1*w/2 < x2 - w/2
      //a1 < (x2 - x1 - w/2)*(2/w)
      //a1 < (2*(x2-x1) - w)/w
      //Absolute value of (x2-x1) to consider all cases automatically

      IT min_bound(1234567890);
      for(int j = 0; j < m; j++){
        int x_dist_2 = 2*(abs(new_posters[i].x - old_posters[j].x)) - w;
        int y_dist_2 = 2*(abs(new_posters[i].y - old_posters[j].y)) - h;

        //Chosing the max between x_dist_2 and y_dist_2 as above
        if(min_bound > std::max(IT(y_dist_2, h), IT(x_dist_2, w))){
          min_bound = std::max(IT(y_dist_2, h), IT(x_dist_2, w));
        }
      }
      if(m > 0){  //Avoid inserting an incorrect constraint
        lp.set_u(i, true, min_bound);
      }
    }

    for(int i = 0; i < n; i++){
      //Minimize the opposite to maximize the objective function
      lp.set_c(i, -2*(h+w));
    }

    Solution s = CGAL::solve_linear_program(lp, ET());

    std::cout<<-floor_to_long(s.objective_value())<<"\n";

  }
}
