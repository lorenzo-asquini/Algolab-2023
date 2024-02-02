#include <iostream>
#include <vector>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K> Triangulation;

typedef K::Point_2 P;

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
      int x, y;
      std::cin>>x>>y;

      points[i] = P(x, y);
    }

    Triangulation t;
    t.insert(points.begin(), points.end());

    int m;
    std::cin>>m;
    for(int i = 0; i < m; i++){
      int x, y;
      std::cin>>x>>y;
      P new_res = P(x, y);

      auto near = t.nearest_vertex(new_res);
      std::cout<<(long)CGAL::squared_distance(new_res, near->point())<<"\n";
    }
  }
}
