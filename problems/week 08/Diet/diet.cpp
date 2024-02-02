#include <iostream>

#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpq.h>

typedef int IT;
typedef CGAL::Gmpz ET;

typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

long floor_to_long(const CGAL::Quotient<ET> &x){
  double a = std::floor(CGAL::to_double(x));
  while (a > x) a -= 1;
  while (a + 1 <= x) a += 1;
  return a;
}


int main(){
  std::ios_base::sync_with_stdio(false);

  while(true){
    int n, m; //n = nutrients, m = foods
    std::cin>>n>>m;

    if(n == 0 && m == 0){
      break;
    }

    //Each food can be taken in only non-negative amounts
    Program lp (CGAL::SMALLER, true, 0, false, 0);

    for(int i = 0; i < n; i++){
      int min, max;
      std::cin>>min>>max;

      lp.set_b(2*i, -min);
      lp.set_b(2*i+1, max);
    }

    for(int j = 0; j < m; j++){
      int p;
      std::cin>>p;

      lp.set_c(j, p);

      for(int i = 0; i < n; i++){
        int c;
        std::cin>>c;

        lp.set_a(j, 2*i, -c);
        lp.set_a(j, 2*i+1, c);
      }
    }

    Solution s = CGAL::solve_linear_program(lp, ET());

    if(s.is_infeasible()){
      std::cout<<"No such diet.\n";
    }else{
      std::cout<<floor_to_long(s.objective_value())<<"\n";
    }
  }
}
