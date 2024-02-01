#include <iostream>

int main(){
  std::ios_base::sync_with_stdio(false);
  
  int t;
  std::cin>>t;
  while(t--){
    int n;
    std::cin>>n;
    
    //Count how many even and odd partial sums are created
    int sum = 0;
    int evens = 0;
    int odds = 0;
    
    for(int i = 0; i < n; i++){
      int x;
      std::cin>>x;
      
      sum += x;
      
      if(sum % 2 == 0){
        evens++;
      }else{
        odds++;
      }
    }
    
    std::cout<< evens*(evens-1)/2 + odds*(odds-1)/2 + evens <<"\n";
  }
}