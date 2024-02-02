#include <iostream>
#include <vector>
#include <algorithm>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;

typedef CGAL::Triangulation_vertex_base_with_info_2<int, K> Vb;
typedef CGAL::Triangulation_face_base_2<K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb, Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds> Triangulation;

typedef K::Point_2 P;

long hours(double death_d){
  //r = t^2+0.5, meaning that r^2 = (t^2+0.5)^2
  //(t^2+0.5) = sqrt(r^2) -> t^2 = sqrt(r^2)-0.5 -> t = sqrt(sqrt(r^2)-0.5)
  double hours = std::max(0.0, std::sqrt(std::sqrt(death_d)-0.5));
  return std::ceil(hours);
}

int main(){
  std::ios_base::sync_with_stdio(false);

  while(true){
    int n;
    std::cin>>n;

    if(n == 0){
      break;
    }

    long l, b, r, t;
    std::cin>>l>>b>>r>>t;

    std::vector<std::pair<P, int>> germs(n);
    std::vector<double> death(n);  //Store the death time of each germ
    for(int i = 0; i < n; i++){
      long x, y;
      std::cin>>x>>y;

      //Find the first death by border
      double border_death = std::pow(x-l, 2);
      border_death = std::min(border_death, std::pow(r-x, 2));
      border_death = std::min(border_death, std::pow(y-b, 2));
      border_death = std::min(border_death, std::pow(t-y, 2));
      death[i] = border_death;

      germs[i] = {P(x, y), i};
    }

    Triangulation tri;
    tri.insert(germs.begin(), germs.end());

    for (auto e = tri.finite_edges_begin(); e != tri.finite_edges_end(); e++) {
      int i1 = e->first->vertex((e->second+1)%3)->info();
      int i2 = e->first->vertex((e->second+2)%3)->info();

      double death_germ = tri.segment(e).squared_length()/4.0;

      //Update the death time of the two germs connected by the current edge
      death[i1] = std::min(death[i1], death_germ);
      death[i2] = std::min(death[i2], death_germ);
    }

    std::sort(death.begin(), death.end());

    std::cout<<hours(death[0])<<" ";
    std::cout<<hours(death[n/2])<<" ";
    std::cout<<hours(death[n-1])<<"\n";
  }
}
