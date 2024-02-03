# Worldcup - solution

Even though there are a lot of variables (a maximum of $4000$), the number of inequalities is low enough to allow for the use of LP.

Save the revenues multiplied by $100$, in order to use only integers in the solver. This means that, when crossing a contour, it's necessary to reduce the revenue by $1$ and not by $0.01$, and that, at the end, it's necessary to divide the result by $100$.

In this problem, the fourth test case says that there are a maximum of 100 contours that contain at least a warehouse or a stadium, while all the others can be ignored. To find the relevant contours, it's necessary to create a triangulation composed of all the locations of the warehouses and the stadiums (without considering differences between the two). After this, for each center of each contour, it's necessary to find the closest vertex in the triangulation and see if the distance to that point is less than the radius (using squared values). If so, the contour should be kept; otherwise, it should be discarded.

For each relevant contour, it's necessary to see if, for each warehouse and for each stadium, one of the two is inside the contour and one is outside. If so, the revenue should be lowered because a contour is crossed.

In the definition of the LP, each variable denotes the number of liters sent from one warehouse to one stadium. The total number of liters sent to a stadium (from all the warehouses) needs to be equal to the demand number, so it's necessary to use two inequalities for each stadium (one with less or equal, one with greater or equal). For the maximum alcohol amount, it's necessary to just use one inequality (pay attention to the fact that the amount of alcohol in the beers is a percentage; the maximum amount is in liters). The number of liters shipped from each warehouse must not exceed the supply.