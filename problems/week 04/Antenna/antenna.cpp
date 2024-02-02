#include <iostream>
#include <vector>

#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
#include <CGAL/Min_circle_2.h>
#include <CGAL/Min_circle_2_traits_2.h>

typedef CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt K;
typedef CGAL::Min_circle_2_traits_2<K> Traits;
typedef CGAL::Min_circle_2<Traits> Min_circle;

typedef K::Point_2 P;

long ceil_to_long(K::FT x){
  double a = std::floor(CGAL::to_double(x));
  while (a < x) a += 1;
  while (a-1 >= x) a -= 1;
  return a;
}

int main(){
  std::ios_base::sync_with_stdio(false);

  while(true){
    int n;
    std::cin>>n;

    if(n == 0){
      break;
    }

    std::vector<P> points(n);
    for(int i = 0; i < n; i++){
      long x, y;
      std::cin>>x>>y;

      points[i] = P(x, y);
    }

    Min_circle mc(points.begin(), points.end(), true);
    Traits::Circle c = mc.circle();

    std::cout<<ceil_to_long(CGAL::sqrt(c.squared_radius()))<<"\n";
  }
}
