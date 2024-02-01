# James Bond's sovereigns

For each possible coin configuration, the player $k$ takes the coin that will make it the most amount of money, while the other players take the coin that will make the player $k$ make the least amount of money. The fact that it says _regardless_ means that it's the maximum amount of money that could be made by the player $k$ in the worst-case scenario where all other players play against him.

In the base case, if there is only one coin left, the value to store is 0 if the last player is not $k$, and it's the single remaining coin if the last player is $k$. In all the other cases, if it's the turn of the player $k$, $a[i][j] = \max(coin[i] + a[i+1][j], coin[j] + a[i][j-1])$. If it is not his turn, then $a[i][j] = \min(a[i+1][j], a[i][j-1])$.