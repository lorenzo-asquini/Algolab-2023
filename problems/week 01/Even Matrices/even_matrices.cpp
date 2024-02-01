#include <iostream>
#include <vector>

int main(){
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin>>t;
  while(t--){
    int n;
    std::cin>>n;

    std::vector<std::vector<int>> matrix(n+2, std::vector<int>(n+2, 0));
    for(int i = 1; i < n+1; i++){
      int par_sum = 0;
      for(int j = 1; j < n+1; j++){
        int x;
        std::cin>>x;

        par_sum += x;
        matrix[i][j] = matrix[i-1][j] + par_sum;
      }
    }

    int tot_even = 0;
    for(int i1 = 1; i1 < n+1; i1++){
      for(int i2 = i1+1; i2 < n+2; i2++){

        int even = 0;
        int odd = 0;
        for(int j = 1; j < n+1; j++){
          int sum = matrix[i2][j] - matrix[i1][j];

          if(sum % 2 == 0){
            even++;
          }else{
            odd++;
          }
        }

        tot_even += (even + even*(even-1)/2 + odd*(odd-1)/2);
      }
    }

    std::cout<<tot_even<<"\n";
  }
}
