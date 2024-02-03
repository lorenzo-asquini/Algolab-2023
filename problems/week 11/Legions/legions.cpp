#include <iostream>
#include <algorithm>

#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

typedef long IT;
typedef CGAL::Gmpz ET;

typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

int main(){
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin>>t;
  while(t--){
    long x, y, n;  //(x, y) = starting position. n = number of legions
    std::cin>>x>>y>>n;

    const int px = 0;
    const int py = 1;
    const int prep = 2;

    Program lp (CGAL::SMALLER, false, 0, false, 0);

    for(int i = 0; i < n; i++){
      long a, b, c, v;
      std::cin>>a>>b>>c>>v;

      long k = std::sqrt(a*a+b*b);

      //The target point P being reachable from (x, y) means that no legion is between the starting point and P.
      //This means that they are on the same side of the line

      //If relative position is greater than 0, the absolute value can be removed without any other change
      //The equation will be: time * v * sqrt(...) <= a*x+b*y+c.
      //The distance must have an upper limit because it need to be maximized
      if(a*x + b*y + c >= 0){
        lp.set_a(prep, i, v*k);
        lp.set_a(px, i, -a);
        lp.set_a(py, i, -b);

        lp.set_b(i, c);
      }else{   //Invert the signs in the absolute value
        lp.set_a(prep, i, v*k);
        lp.set_a(px, i, a);
        lp.set_a(py, i, b);

        lp.set_b(i, -c);
      }
    }
    //Time needs to be positive
    lp.set_l(prep, true, 0);

    //Maximize time
    lp.set_c(prep, -1);

    Solution s = CGAL::solve_linear_program(lp, ET());
    //Division is good enough to find lower bound
    std::cout << -s.objective_value_numerator()/s.objective_value_denominator() << "\n";
  }
}
