#include <iostream>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel EPIC;
typedef EPIC::Segment_2 S;
typedef EPIC::Ray_2 R;
typedef EPIC::Point_2 P;

int main(){
  std::ios_base::sync_with_stdio(false);

  while(true){
    int n;
    std::cin>>n;

    if(n == 0){
      break;
    }

    long x, y, a, b;
    std::cin>>x>>y>>a>>b;

    R ray(P(x,y), P(a, b));

    bool collision = false;
    for(int i = 0; i < n; i++){
      long r, s, t, u;
      std::cin>>r>>s>>t>>u;

      if(!collision){
        S segment(P(r, s), P(t, u));

        if(CGAL::do_intersect(ray, segment)){
          collision = true;
        }
      }
    }

    if(collision){
      std::cout<<"yes\n";
    }else{
      std::cout<<"no\n";
    }
  }
}
