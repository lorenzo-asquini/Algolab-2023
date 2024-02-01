#include <iostream>
#include <vector>
#include <algorithm>

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef K::Ray_2 R;
typedef K::Point_2 P;
typedef K::Segment_2 S;

long floor_to_long(const K::FT& x){
  double a = std::floor(CGAL::to_double(x));
  while (a > x) a -= 1;
  while (a+1 <= x) a += 1;
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

    long x, y, a, b;
    std::cin>>x>>y>>a>>b;

    R ray(P(x, y), P(a, b));

    std::vector<S> segments(n);
    for(int i = 0; i < n; i++){
      long r, s, t, u;
      std::cin>>r>>s>>t>>u;

      segments[i] = S(P(r, s), P(t, u));
    }

    //Shuffle the input segments to avoid the worst case
    std::random_shuffle(segments.begin(), segments.end());

    bool first_intersection = false;
    S segment_ray;

    for(int i = 0; i < n; i++){
      //Use the ray until an intersection is found
      if(!first_intersection && CGAL::do_intersect(ray, segments[i])){

        auto o = CGAL::intersection(ray, segments[i]);

        //Create a segment to make the ray smaller
        if (const P* op = boost::get<P>(&*o)){
          segment_ray = S(P(x, y), *op);

        }else if (const S* os = boost::get<S>(&*o)){
          //If the intersection is an entire segment, find which one of the points of the segment is closer
          K::FT d_source = CGAL::squared_distance(os->source(), P(x, y));
          K::FT d_target = CGAL::squared_distance(os->target(), P(x, y));

          if(d_source < d_target){
            segment_ray = S(P(x, y), os->source());
          }else{
            segment_ray = S(P(x, y), os->target());
          }
        }
        first_intersection = true;
      }else if(first_intersection && CGAL::do_intersect(segment_ray, segments[i])){
        //Use the smaller segment to find intersections
        auto o = CGAL::intersection(segment_ray, segments[i]);

        //Decrease the size of the segment
        if (const P* op = boost::get<P>(&*o)){
          segment_ray = S(P(x, y), *op);

        }else if (const S* os = boost::get<S>(&*o)){
          K::FT d_source = CGAL::squared_distance(os->source(), P(x, y));
          K::FT d_target = CGAL::squared_distance(os->target(), P(x, y));

          if(d_source < d_target){
            segment_ray = S(P(x, y), os->source());
          }else{
            segment_ray = S(P(x, y), os->target());
          }
        }
      }
    }

    if(!first_intersection){
      std::cout<<"no\n";
    }else{
      P point = segment_ray.target();
      std::cout<<floor_to_long(point.x())<<" "<<floor_to_long(point.y())<<"\n";
    }
  }
}
