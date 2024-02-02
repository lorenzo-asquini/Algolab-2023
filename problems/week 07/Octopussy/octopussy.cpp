#include <iostream>
#include <vector>
#include <cmath>
#include <set>
#include <limits>
#include <algorithm>

typedef struct{
  bool defused;  //If it was defused
  int index;
  int exp_t;  //Time at which it explodes
} bomb_t;

//Place before the bombs that explode first
struct bombComp{
  bool operator()(bomb_t b1, bomb_t b2){
    return b1.exp_t < b2.exp_t;
  }
};

typedef std::vector<bomb_t> VB;
typedef std::vector<VB> VVB;

VB order_bombs_below(int index, VB &bombs, VVB &ordered_bombs_below, int n){

  //Indexes of the bombs below the current one
  int left_index = 2*index+1;
  int right_index = 2*index+2;

  //There are no bombs below, return a vector containing only the current bomb
  if(left_index >= n){
    VB vector_to_return;
    vector_to_return.push_back(bombs[index]);
    return vector_to_return;
  }

  //Already figured out the bombs needed to defuse before the current one
  if(ordered_bombs_below[index].size() != 0){
    return ordered_bombs_below[index];
  }

  //Find all the bombs that needs to be defused to defuse the left and right bombs below
  VB left_vector = order_bombs_below(left_index, bombs, ordered_bombs_below, n);
  VB right_vector = order_bombs_below(right_index, bombs, ordered_bombs_below, n);

  //The bombs below are the union of the left vector and the right vector
  left_vector.insert(left_vector.end(), right_vector.begin(), right_vector.end());

  std::sort(left_vector.begin(), left_vector.end(), bombComp());
  ordered_bombs_below[index] = left_vector;

  //Return the set containing also the current bomb
  //The last bomb is not ordered, but it will be ordered in the caller function
  left_vector.push_back(bombs[index]);
  return left_vector;
}

int defuse_pyramid(int index, VB &bombs, VVB &ord_bomb_below, int time_elapsed, bool &explosion){
  //Does not matter the time anymore
  if(explosion){
    return -1;
  }

  //Do not defuse if already defused
  if(bombs[index].defused){
    return time_elapsed;
  }

  //Defuse all bombs below
  for(auto it = ord_bomb_below[index].begin(); it != ord_bomb_below[index].end(); it++){
    if(!bombs[it->index].defused){
      time_elapsed = defuse_pyramid(it->index, bombs, ord_bomb_below, time_elapsed, explosion);
    }
  }

  //Defuse the current bomb if possible
  if(time_elapsed < bombs[index].exp_t){
    bombs[index].defused = true;
    time_elapsed++;
  }else{
    explosion = true;
  }

  return time_elapsed;
}

int main(){
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin>>t;
  while(t--){
    int n;
    std::cin>>n;

    VB bombs(n);
    for(int i = 0; i < n; i++){
      int e;
      std::cin>>e;

      bombs[i] = {false, i, e};
    }

    //For each bomb, there is a vector containing the bombs below it ordered by the explosion time
    //The bombs are stored in an ordered vector and not a muliset because it's faster to order once than to insert ordered
    VVB ordered_bombs_below(n);
    VB defuse_order = order_bombs_below(0, bombs, ordered_bombs_below, n);
    std::sort(defuse_order.begin(), defuse_order.end(), bombComp());

    int time_elapsed = 0;
    bool explosion = false;

    for(int i = 0; i < n; i++){

      //Defuse the bombs according to the optimal order found previously, that tries to defuse first the bombs that explode sooner
      int current_bomb_index = defuse_order[i].index;
      if(!bombs[current_bomb_index].defused){
        time_elapsed = defuse_pyramid(current_bomb_index, bombs, ordered_bombs_below, time_elapsed, explosion);
      }

      if(explosion){
        break;
      }
    }

    if(!explosion){
      std::cout<<"yes\n";
    }else{
      std::cout<<"no\n";
    }
  }
}
