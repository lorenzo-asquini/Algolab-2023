# DHL - solution

# Method 1

Keep in mind that this method is not fast enough for the last test case, which, however, is worth only 1 point.

To limit the amount of computation to do, it's possible to store, in the array of the piles, the sum of the value that comes from taking all the top $i$ boxes (from the first box on the top to the one considered). This allows to find in $O(1)$ the sum of the values of a subset of boxes with a difference.

Create a DP table `dp[i][j]` that represents the minimum cost when there are `i` boxes A left and `j` boxes B left. Considering that it's necessary to take at least one box, the DP table is initialized, computing the cost of taking the last box from a stack and all the combinations of boxes from the other stack (that contains a maximum of $n-1$ boxes because at least one turn was performed).

To populate the DP table, for the remaining boxes in stack A ($2$ to $n$), for the remaining boxes in stack B ($2$ to $n$), take 1 box from pile A and iterate over the remaining boxes from pile B ($1$ to $remB-1$), or take 1 box from pile B and iterate over the remaining boxes from pile A ($1$ to $remA-1$). Find out if it's better to use the current saved value for that configuration of remaining boxes or take the boxes decided in the current iteration and add up the best value that comes from taking the remaining boxes.

The idea that allows to solve the problem easily is that the optimal solution involves taking only one box from a stack and any feasible amount from the other stack. This is because it's better to sum little multiplications than have fewer larger multiplications.

# Fast method

Black magic.