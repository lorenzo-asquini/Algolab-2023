#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <climits>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel EPIC;

typedef CGAL::Triangulation_vertex_base_2<EPIC> Vb;
typedef CGAL::Triangulation_face_base_with_info_2<EPIC::FT, EPIC> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb, Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<EPIC, Tds> Triangulation;

typedef EPIC::Point_2 P;

int main(){
  std::ios_base::sync_with_stdio(false);

  while(true){
    int n;
    std::cin>>n;

    if(n == 0){
      break;
    }

    std::vector<P> infected(n);
    for(int i = 0; i < n; i++){
      int x, y;
      std::cin>>x>>y;

      infected[i] = P(x, y);
    }

    //Triangulation of the infected people
    Triangulation t;
    t.insert(infected.begin(), infected.end());

    //Priority queue that contains pairs.
    //The default behaviour is ordering the pairs by the first element of each pair
    std::priority_queue<std::pair<long, Tds::Face_handle>> q;
    for (auto f = t.all_faces_begin(); f != t.all_faces_end(); f++){
      if(t.is_infinite(f)){
        f->info() = LONG_MAX;
      }else{
        f->info() = 0;

        for(int i = 0; i < 3; i++){
          auto neighbor = f->neighbor(i);

          if(t.is_infinite(neighbor)){
            //Associate with each face near the border the space available to escape to infinity
            q.push({t.segment(f, i).squared_length(), f});
          }
        }
      }
    }

    while(!q.empty()){

      auto current = q.top();
      q.pop();

      //Face was already considered or is an infinite face
      if(current.second->info() != 0){
        continue;
      }

      //Associate to each face the space available to escape to infinity
      current.second->info() = current.first;

      for(int i = 0; i < 3; i++){
        auto neigh = current.second->neighbor(i);

        if(!t.is_infinite(neigh)){
          //The space available to escape from a face is the bottleneck
          //of the best path from the current face to infinity
          long edge = t.segment(current.second, i).squared_length();
          long best = std::min(current.first, edge);

          q.push({best, neigh});
        }
      }
    }

    int m;
    std::cin>>m;
    for(int i = 0; i < m; i++){
      long x, y, d;
      std::cin>>x>>y>>d;

      auto nearest = t.nearest_vertex(P(x, y));
      long dist = CGAL::squared_distance(nearest->point(), P(x, y));

      //Check if the current person is already too close to an infected person
      if(dist < d){
        std::cout<<"n";
        continue;
      }

      //Check if the best path to infinity is large enough
      auto locate = t.locate(P(x, y));
      if(locate->info() < 4*d){
        std::cout<<"n";
      }else{
        std::cout<<"y";
      }
    }
    std::cout<<"\n";
  }
}
