# Return of the Jedi - solution

The solution involves finding the second-best MST.

## Method 1

Create an undirected weighted graph using BGL. At the same time, save in a separate matrix (using vectors) all the costs of the edges. After finding the MST using the BGL function, create a matrix (using vectors) that contains all the undirected edges of the MST. At the same time, mark if an edge is inside the MST and calculate the cost of the MST.

Using DFS, calculate for each pair of nodes $u$ and $v$ in the MST the most expensive edge in the MST between $u$ and $v$. Given the starting node $u$, to find the most expensive edge to reach $w$, which is a neighbor of the current node $v$, it's necessary to find the maximum between the cost of $(v, w)$ and the maximum edge cost to reach $v$.

For all the edges $(u, v)$ in the original graph that are not in the MST, search for the one that minimizes the difference between adding the new edge and removing the most expensive edge between $u$ and $v$ in the MST. Adding the new edge creates a cycle that can be broken by removing the most expensive edge before the addition of the new one.

The final result is the sum between the MST cost and the additional cost that comes from swapping the selected edges.

## Method 2

Save all the edges of the graph in a vector and sort them by their cost.

In order to find the second-best MST, it's possible to iteratively create a connected graph by skipping each one of the edges of the MST at a time and substituting it with the cheapest edge that connects the two connected components that would have been connected by the skipped edge. An inexpensive method to achieve this is to use disjointed sets with storage, iterating over all the edges of the original graph ordered by their cost, and, when an edge connects two connected components (an edge of the MST), keeping it or skipping it according to which edge needs to be skipped according to the main loop.