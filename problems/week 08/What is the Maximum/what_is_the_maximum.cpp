#include <iostream>

#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

typedef int IT;
typedef CGAL::Gmpz ET;

typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

long floor_to_long(const CGAL::Quotient<ET> &x){
  double a = std::floor(CGAL::to_double(x));
  while (a > x) a -= 1;
  while (a+1 <= x) a += 1;
  return a;
}

long ceil_to_long(const CGAL::Quotient<ET> &x){
  double a = std::ceil(CGAL::to_double(x));
  while (a < x) a += 1;
  while (a-1 >= x) a -= 1;
  return a;
}

int main(){
  std::ios_base::sync_with_stdio(false);

  while(true){
    int p;
    std::cin>>p;

    if(p == 0){
      break;
    }

    int a, b;
    std::cin>>a>>b;

    const int X = 0;
    const int Y = 1;
    const int Z = 2;

    if(p == 1){
      Program lp (CGAL::SMALLER, true, 0, false, 0);

      //x+y >= -4
      lp.set_a(X, 0, 1); lp.set_a(Y, 0, 1); lp.set_b(0, 4);

      //4x+2y <= ab
      lp.set_a(X, 1, 4); lp.set_a(Y, 1, 2); lp.set_b(1, a*b);

      //−x+y <= 1
      lp.set_a(X, 2, -1); lp.set_a(Y, 2, 1); lp.set_b(2, 1);

      //Max (by - ax)  --> Min (-by + ax)
      lp.set_c(Y, -b); lp.set_c(X, a);

      Solution s = CGAL::solve_linear_program(lp, ET());

      if(s.is_unbounded()){
        std::cout<<"unbounded\n";
      }else if(s.is_infeasible()){
        std::cout<<"no\n";
      }else{
        std::cout<<floor_to_long(-s.objective_value())<<"\n";
      }
    }else{
      Program lp (CGAL::SMALLER, false, 0, true, 0);

      //x+y <= -4 --> -x-y <= 4
      lp.set_a(X, 0, -1); lp.set_a(Y, 0, -1); lp.set_b(0, 4);

      //4x+2y+z >= -ab --> -4x-2y-z <= ab
      lp.set_a(X, 1, -4); lp.set_a(Y, 1, -2); lp.set_a(Z, 1, -1); lp.set_b(1, a*b);

      //−x+y >= -1 --> x-y <= 1
      lp.set_a(X, 2, 1); lp.set_a(Y, 2, -1); lp.set_b(2, 1);

      //Min ax+by+z
      lp.set_c(X, a); lp.set_c(Y, b); lp.set_c(Z, 1);

      Solution s = CGAL::solve_linear_program(lp, ET());

      if(s.is_unbounded()){
        std::cout<<"unbounded\n";
      }else if(s.is_infeasible()){
        std::cout<<"no\n";
      }else{
        std::cout<<ceil_to_long(s.objective_value())<<"\n";
      }
    }
  }
}
