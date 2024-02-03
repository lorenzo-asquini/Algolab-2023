#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>

typedef CGAL::Exact_predicates_exact_constructions_kernel EPEC;  //Use exact for very big numbers

typedef std::pair<bool, EPEC::FT> Info;
typedef CGAL::Triangulation_vertex_base_2<EPEC> Vb;
typedef CGAL::Triangulation_face_base_with_info_2<Info, EPEC> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb, Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<EPEC, Tds> Triangulation;

typedef EPEC::Point_2 P;
typedef EPEC::FT FT;

int main(){
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin>>t;
  while(t--){
    long n, m, d;  //n = dementors, m = fugitives, d = moving distance dementors daytime
    std::cin>>n>>m>>d;

    std::vector<P> dementors(n);
    for(int i = 0; i < n; i++){
      long x, y;
      std::cin>>x>>y;

      dementors[i] = P(x, y);
    }

    Triangulation tri;
    tri.insert(dementors.begin(), dementors.end());

    //It's necessary to have the faces ordered by their safe distance in order to consider the worst case scenario
    //Store a pair with the first element the value that needs to be ordered to have automatic ordering in the priority queue
    std::priority_queue<std::pair<FT, Triangulation::Face_handle>> q;
    for (auto f_it = tri.finite_faces_begin(); f_it != tri.finite_faces_end(); f_it++){

      //The safest spot inside a face is the point in the Voronoi diagram that is the dual of the face
      //Being the spot equally far from all the vertices of the face triangle, it's the safest spot
      //To calculate the distance to any dementor, it's possible to chose any vertex in the face triangle
      FT safe_inside_face = CGAL::squared_distance(f_it->vertex(0)->point(), tri.dual(f_it));

      //For now consider the Voronoi dual as the safest spot to be
      f_it->info() = {false, safe_inside_face};

      for(int i = 0; i < 3; i++){
        auto neighbor = f_it->neighbor(i);

        if(!tri.is_infinite(neighbor)){  //Consider only direct exits for now
          continue;
        }

        //For the faces near the infinite space, decide if it's safer to remain inside the face
        //or to exit in the infinite space.
        //Safest means being the least close to any dementor at any time
        f_it->info().second = std::max(f_it->info().second, tri.segment(f_it, i).squared_length());
      }

      q.push({f_it->info().second, f_it});
    }

    while(!q.empty()){
      auto current_face = q.top().second;
      q.pop();

      if(!current_face->info().first){  //Face already computed

        //Update the neighbors
        for (int i = 0; i < 3; i++) {
          auto neighbor = current_face->neighbor(i);

          if (tri.is_infinite(neighbor)){
            continue;
          }

          if(!neighbor->info().first){

            //Find which is the most limiting distance when choosing to go from neighbor to the current face
            //The limiting factor may be going from the neighbor to the current face, or the current face itself
            FT limit_neighbor_to_current = std::min(tri.segment(current_face, i).squared_length(), current_face->info().second);

            //Find if the neighbor is already safer, or it's better to go to the current face
            neighbor->info().second = std::max(neighbor->info().second, limit_neighbor_to_current);

            q.push({neighbor->info().second, neighbor});
          }
        }
        current_face->info().first = true;
      }
    }

    for(int i = 0; i < m; i++){
      long x, y, s;
      std::cin>>x>>y>>s;
      P fugitive(x, y);

      //Calculate which is the safe radius. It is equivalent to (s+d)^2, but it has no overflow errors
      FT r = CGAL::squared_distance(P(0, 0), P(s + d, 0));

      //Fugitive is already too close
      if(CGAL::squared_distance(fugitive, tri.nearest_vertex(fugitive)->point()) < r){
        std::cout<<"n";
        continue;
      }

      auto f = tri.locate(fugitive);
      if(tri.is_infinite(f)){
        std::cout << "y";

      }else if (f->info().second < 4*r){  //During the night, the safe spot must be 2*(d+s) from dementors
        std::cout << "n";

      }else{
        std::cout << "y";
      }
    }

    std::cout<<"\n";
  }
}
