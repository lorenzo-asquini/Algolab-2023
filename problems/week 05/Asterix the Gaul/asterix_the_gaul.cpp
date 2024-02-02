#include <iostream>
#include <vector>
#include <algorithm>

typedef struct{
  long d;
  long t;
} movement_t;

//Shortest time first, then longest distance
struct orderMovements{
  bool operator()(movement_t &m1, movement_t &m2){
    if (m1.t == m2.t) {
        return m1.d > m2.d;
    }
    return m1.t < m2.t;
  }
};

bool solve(std::vector<movement_t> &movements, int n, long D, long T, long gulp){

  //Split and list
  int n1 = n/2;
  int n2 = n-n1;

  std::vector<movement_t> list1;
  for(int i = 0; i < (1<<n1); i++){

    long d_sum = 0;
    long t_sum = 0;

    for(int j = 0; j < n1; j++){
      if(i & (1<<j)){
        d_sum += movements[j].d + gulp;  //Add potion each step
        t_sum += movements[j].t;
      }
    }

    list1.push_back({d_sum, t_sum});
  }

  std::vector<movement_t> list2;
  for(int i = 0; i < (1<<n2); i++){

    long d_sum = 0;
    long t_sum = 0;
    int steps = 0;

    for(int j = 0; j < n2; j++){
      if(i & (1<<j)){
        d_sum += movements[n1+j].d + gulp;  //Add potion each step
        t_sum += movements[n1+j].t;
        steps++;
      }
    }

    list2.push_back({d_sum, t_sum});
  }

  std::sort(list1.begin(), list1.end(), orderMovements());
  std::sort(list2.begin(), list2.end(), orderMovements());

  //The partial sums are ordered first my the time (less time first), then by the distance (more distance first)
  //For this reason, if there are cases when less space is traversed in more time, that should not be considered

  //If while traversing the vector there an instance where the distance traversed is less than previously found,
  //that value is discarded because, considering the ordering, the speed will be for sure lower than previously found
  long max_d = 0;
  std::vector<movement_t> list1_cleaned;
  for (auto mov : list1){
    if(mov.d > max_d) {
      max_d = mov.d;
      list1_cleaned.push_back(mov);
    }
  }

  max_d = 0;
  std::vector<movement_t> list2_cleaned;
  for (auto mov : list2){
    if(mov.d > max_d) {
      max_d = mov.d;
      list2_cleaned.push_back(mov);
    }
  }

  bool found = false;
  int i1 = 0, i2 = list2_cleaned.size() - 1;

  //Two sum technique
  while (i1 < (int)list1_cleaned.size() && i2 >= 0){

    long tot_D = list1_cleaned[i1].d + list2_cleaned[i2].d;
    long tot_T = list1_cleaned[i1].t + list2_cleaned[i2].t;

    if (tot_D >= D){
      if(tot_T < T){
        found = true;
        break;
      }else{
        i2--;
      }
    }else{
      i1++;
    }
  }

  //Return if it's possible to reach Panoramix in time
  return found;
}

int main(){
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin>>t;
  while(t--){
    int n, m;  //n = movements, m = gulps
    long D, T;  //D = distance, T = max time
    std::cin>>n>>m>>D>>T;

    std::vector<movement_t> movements(n);
    for(int i = 0; i < n; i++){
      long d, t;
      std::cin>>d>>t;

      movements[i] = {d, t};
    }

    std::vector<long> gulps(m+1);
    gulps[0] = 0;
    for(int i = 1; i < m+1; i++){
      long s;
      std::cin>>s;

      gulps[i] = s;
    }

    //Binary search on the number of gulps to take
    int l = 0; int h = m;
    while(l < h){
      int mid = (l+h)/2;
      long gulp = gulps[mid];

      if(solve(movements, n, D, T, gulp)){
        h = mid;
      }else{
        l = mid+1;
      }
    }

    if(solve(movements, n, D, T, gulps[l])){
      std::cout<<l<<"\n";
    }else{
      std::cout<<"Panoramix captured\n";
    }
  }
}
