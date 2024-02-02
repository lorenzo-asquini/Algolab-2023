#include <iostream>
#include <vector>
#include <set>

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>

typedef CGAL::Exact_predicates_exact_constructions_kernel K;

typedef struct{
  int index;  //Index in the original array
  long y;
  K::FT slope;  //Precise value
} biker_t;

//Order bikers by placing the highest first
struct compMoto{
  bool operator()(biker_t &m1, biker_t &m2){
    return m1.y > m2.y;
  }
};

int main(){
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin>>t;
  while(t--){
    int n;
    std::cin>>n;

    std::vector<biker_t> bikers(n);
    for(int i = 0; i < n; i++){
      long y0, x1, y1;
      std::cin>>y0>>x1>>y1;

      bikers[i] = {i, y0, K::FT(y1-y0)/K::FT(x1)};
    }

    std::sort(bikers.begin(), bikers.end(), compMoto());

    //Store the indices of the bikers that go on forever relative to the sorted array
    std::vector<int> forever;
    for(int i = 0; i < n; i++){

      while(true){

        //If the slope of the new biker is smaller than the one of the previous biker, there are no collisions
        if(forever.size() == 0 || bikers[i].slope <= bikers[forever.back()].slope){
          forever.push_back(i);
          break;
        }

        //There is a collision
        int last_i = forever.back();

        if(CGAL::sign(bikers[i].slope) == CGAL::sign(bikers[last_i].slope)){
          if(CGAL::sign(bikers[i].slope) == CGAL::NEGATIVE){

            //Current slope higher than the previous one, with same negative signs. The current one arrives sooner
            forever.pop_back();
            continue;
          }else{
            //Current slope higher than the previous one, with same positive signs. The current one arrives later
            break;
          }

        }else{
          if(CGAL::abs(bikers[i].slope) <= CGAL::abs(bikers[last_i].slope)){

            //Current slope higher than the previous one, but with a lower absolute value, with different signs.
            //The current one arrives sooner
            forever.pop_back();
            continue;
          }else{

            //Current slope higher than the previous one, but with a higher absolute value, with different signs.
            //The current one arrives later
            break;
          }
        }
      }
    }

    //Retrieve the indexes considering the original ordering
    std::set<int> indices;
    for(int i: forever){
      indices.insert(bikers[i].index);
    }

    for(int i: indices){
      std::cout<<i<<" ";
    }
    std::cout<<"\n";
  }
}
