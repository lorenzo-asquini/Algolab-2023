#include <iostream>
#include <vector>
#include <algorithm>

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef K::Point_2 P;
typedef K::Line_2 L;

int main(){
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin>>t;
  while(t--){
    int m, n; //m-1 = legs, n = map parts
    std::cin>>m>>n;

    std::vector<P> points(m);
    for(int i = 0; i < m; i++){
      int x, y;
      std::cin>>x>>y;

      points[i] = P(x, y);
    }

    //Store which legs each map part contains
    //A leg is identified by the index of its first poin
    std::vector<std::vector<int>> contained_segments(n);
    for(int i = 0; i < n; i++){
      int a, b, c, d;
      std::cin>>a>>b>>c>>d;
      L side1 = L(P(a, b), P(c, d));

      std::cin>>a>>b>>c>>d;
      L side2 = L(P(a, b), P(c, d));

      std::cin>>a>>b>>c>>d;
      L side3 = L(P(a, b), P(c, d));

      //Find the vertices of the triangle parts
      auto o1 = CGAL::intersection(side1, side2);
      P* v1 = boost::get<P>(&*o1);

      auto o2 = CGAL::intersection(side2, side3);
      P* v2 = boost::get<P>(&*o2);

      auto o3 = CGAL::intersection(side3, side1);
      P* v3 = boost::get<P>(&*o3);

      //Check which single points are inside the map part
      std::vector<bool> contains(m, false);
      for(int j = 0; j < m; j++){
        int or1 = CGAL::orientation(*v1, *v2, points[j]);
        int or2 = CGAL::orientation(*v2, *v3, points[j]);
        int or3 = CGAL::orientation(*v3, *v1, points[j]);

        if((or1 <= 0 && or2 <= 0 && or3 <= 0) ||
          (or1 >= 0 && or2 >= 0 && or3 >= 0)){
            contains[j] = true;
          }
      }

      for(int j = 0; j < m-1; j++){
        if(contains[j] && contains[j+1]){
          contained_segments[i].push_back(j);
        }
      }
    }

    //Count how many map parts that are currently considered contain a certain leg
    std::vector<int> in_map(m-1, 0);
    int tot_segments = 0;
    int low = 0;
    int high = 0;

    //Insert the legs of the first map part
    for(int leg: contained_segments[0]){
      in_map[leg]++;
      tot_segments++;
    }

    //Sliding window to find the minimum number of map parts to use
    int min_maps = n;
    while(low <= high){
      if(tot_segments == m-1){
        min_maps = std::min(min_maps, high-low+1);

        for(int leg: contained_segments[low]){
          in_map[leg]--;
          if(in_map[leg] == 0){
            tot_segments--;
          }
        }
        low++;
        continue;
      }

      if(tot_segments < m-1 && high < n-1){
        high++;
        for(int leg: contained_segments[high]){
          in_map[leg]++;
          if(in_map[leg] == 1){
            tot_segments++;
          }
        }
      }else{
        for(int leg: contained_segments[low]){
          in_map[leg]--;
          if(in_map[leg] == 0){
            tot_segments--;
          }
        }
        low++;
      }
    }

    std::cout<<min_maps<<"\n";
  }
}
