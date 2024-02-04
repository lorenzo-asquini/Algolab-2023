#include <iostream>
#include <vector>
#include <algorithm>

#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

typedef int IT;
typedef CGAL::Gmpz ET;

typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

typedef struct{
  int f;  //Start filth
  int c;  //Max allowed filth
  long k;
  long l;
  long m;
} stall_t;

int main(){
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin>>t;
  while(t--){
    int n;  //n = stables
    std::cin>>n;

    std::vector<stall_t> stalls(n);
    for(int i = 0; i < n; i++){
      int f, c, k, l, m;
      std::cin>>f>>c>>k>>l>>m;

      stalls[i] = {f, c, k, l, m};
    }

    //Consider also the case where working 0 hours gives 0 more water
    std::vector<long> waterA(25);
    for(int i = 1; i < 25; i++){
      int a;
      std::cin>>a;

      waterA[i] = a + waterA[i-1];
    }

    //Consider also the case where working 0 hours gives 0 more water
    std::vector<long> waterB(25, 0);
    for(int i = 1; i < 25; i++){
      int b;
      std::cin>>b;

      waterB[i] = b + waterB[i-1];
    }

    //Constants for the LP solver
    const int h1 = 0;
    const int h2 = 1;
    const int h3 = 2;

    //Start with a configuration that allows one value only to grow and the other only to shrink
    //This allows to have only one behaviour in case of feasibility or infeasibility
    int workA = 0;
    int workB = 24;

    int min_hours = INT_MAX;
    while(workA <= 24 && workB >= 0){
      //Increasing the number of hours worked on A made the total number of hours worked worse
      if(workA + workB >= min_hours){
        workB--;
        continue;
      }

      //Solve the current configuration
      Program lp (CGAL::SMALLER, true, 0, true, 1);
      for(int i = 0; i < n; i++){
        int k = stalls[i].k + waterA[workA]*waterA[workA];
        int l = stalls[i].l + waterB[workB]*waterB[workB];
        int m = stalls[i].m + waterA[workA]*waterB[workB];

        lp.set_a(h1, i, -k);
        lp.set_a(h2, i, -l);
        lp.set_a(h3, i, -m);
        lp.set_b(i, stalls[i].c - stalls[i].f);
      }

      Solution s = CGAL::solve_linear_program(lp, ET());

      if(s.is_infeasible()){
        //Work more hours. Because of the starting configuration, it's possible to work more only on A
        workA++;
      }else{
        min_hours = std::min(min_hours, workA + workB);

        //Work less hours. Because of the starting configuration, it's possible to work less only on B
        workB--;
      }
    }

    if(min_hours == INT_MAX){
      std::cout<<"Impossible!\n";
    }else{
      std::cout<<min_hours<<"\n";
    }
  }
}
