# India - solution

There is a lot of time, so it's possible to do a binary search on the number of suitcases transported, considering that the maximum number of suitcases that can be transported is equal to the total number of elephants.

In the binary search, create a graph for maximum flow and minimum cost. Connect the source to the first city and the last city to the sink with a capacity equal to the middle value between low and high. Calculate the flow and the cost. If the flow is less than the number of suitcases that the iteration tried to move, then it's necessary to lower the number of suitcases (treat as if the cost were too high). If the cost is higher than the budget, take fewer suitcases. If it's lower, take more suitcases and update the maximum number of suitcases that can be taken.