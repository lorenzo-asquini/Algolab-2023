# Canteen - solution

In the graph, there is a node for each day:
- From the source to each day, the capacity is the number of menus that can be cooked, and the cost is the cost per menu.
- From each day to the sink, the capacity is the number of students that need to be served, and the cost is $20-p$, where $p$ is the profit from selling a single menu. Using $-p$ would mean considering that selling a menu has a negative cost, so it lowers the minimum cost at the end (the goal is to spend as little as possible). It's necessary to add $20$ because using the function `cycle_canceling` that allows for negative costs is too expensive. At the end, the result needs to be adjusted.
- From each day to the next, the capacity is the volume of the freezer, and the cost is the cost of the energy for each menu in the freezer.

The maximum flow is the number of students served. It's necessary to sum up all the numbers of students per day to see if it's possible to serve all the students. The profit, in this case, is the opposite of the total cost. However, it's necessary to consider that the cost was augmented by $20$ for each served student, so it's necessary to add $20$ for each served student to find the final result.