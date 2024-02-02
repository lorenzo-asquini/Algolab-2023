#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>

typedef struct{
  int l;  //Length
  int p;  //Anchoring point
} boat_t;

//Order by the anchoring points of the boats
struct orderBoats{
  bool operator()(boat_t &b1, boat_t &b2){
    return b1.p < b2.p;
  }
};

int main(){
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin>>t;
  while(t--){
    int n;
    std::cin>>n;

    std::vector<boat_t> boats(n);
    for(int i = 0; i < n; i++){
      int l, p;
      std::cin>>l>>p;

      boats[i] = {l, p};
    }

    std::sort(boats.begin(), boats.end(), orderBoats());

    int index = 1;
    int occ = boats[0].p;  //Position until the dock is occupied
    int prev_occ = INT_MIN;  //Position until the dock was occupied before taking the last boat

    int count = 1;

    while(index < n){
      boat_t current_b = boats[index];

      //The current boat fits
      if(current_b.p > occ){
        prev_occ = occ;
        count++;

        //Place the end where the point is
        if(current_b.p - current_b.l > occ){
          occ = current_b.p;
        }else{
          occ = occ + current_b.l;
        }
      }else{  //Boat does not fit

        //Try to see if it's better to replace the previous boat
        if(current_b.p > prev_occ){  //This boat would fit if the previous was not present

          //Try to see if the space occupied would get better
          if(current_b.p - current_b.l > prev_occ && current_b.p < occ){
            occ = current_b.p;
          }else if(prev_occ + current_b.l < occ){
            occ = prev_occ + current_b.l;
          }
        }
      }

      index++;
    }

    std::cout<<count<<"\n";
  }
}
