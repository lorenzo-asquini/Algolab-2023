#include <iostream>
#include <algorithm>

#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

typedef int IT;
typedef CGAL::Gmpz ET;

typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

int main(){
  std::ios_base::sync_with_stdio(false);

  while(true){
    int n;  //n = inequalities
    std::cin>>n;

    if(n == 0){
      break;
    }

    int d;  //d = dimensions
    std::cin>>d;

    Program lp (CGAL::SMALLER, false, 0, false, 0);

    //Variable that is the distance to the planes
    //All the other variables have index from 0 to d-1
    const int D = d;

    for(int i = 0; i < n; i++){

      //Sum of the square roots of the coefficients
      int sq_sum = 0;

      //The distance to a plane (in any dimensions) is:
      //D = |a_1 * x_1 + a_2 * x_2 + ... - b| / sqrt(||a||)

      //Given D * sqrt(||a||) = |y-b|, where y = a_1 * x_1 + a_2 * x_2 + ...
      //Considering that y <= b, we have D * sqrt(||a||) = -y+b
      //It's equal to D * sqrt(||a||) <= -y+b and D * sqrt(||a||) >= -y+b.
      //Considering that it's necessary to maximise D, the first one is chosen

      lp.set_l(D, true, 0);

      for(int j = 0; j < d; j++){
        int a;
        std::cin>>a;

        //Variable j, equation i
        lp.set_a(j, i, a);

        sq_sum += a*a;
      }

      lp.set_a(D, i, std::sqrt(sq_sum));

      int b;
      std::cin>>b;

      lp.set_b(i, b);
    }

    //Maximize the distance to the planes, so minimize the opposite
    lp.set_c(D, -1);

    Solution s = CGAL::solve_linear_program(lp, ET());

    if(s.is_unbounded()){
      std::cout<<"inf\n";
    }else if(s.is_infeasible()){
      std::cout<<"none\n";
    }else{
      std::cout<< -s.objective_value_numerator()/s.objective_value_denominator() <<"\n";
    }
  }
}
