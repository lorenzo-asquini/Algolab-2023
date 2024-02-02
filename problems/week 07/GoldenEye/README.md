# GoldenEye - solution

In order to solve this problem, it's better to take a look at the example code in _emst.cpp_.

Firstly, it's necessary to create the triangulation with the jammer positions. Each vertex of the triangulation needs to have its own index as info. While iterating through all the finite vertices of the triangulation, it's possible to represent them with the indexes of the two vertices they connect:
- An edge is stored as a pair of a face and the index (from 0 to 2) of the vertex that is opposite to the edge in the face:
```
auto face = e->first
```
- If `i` is the vertex in the face that is opposite to the edge, then `(i+1)%3` and `(i+2)%3` are the indexes of the vertices inside the face that are connected by the edge:
```
int vertex_index_1 = (e->second+1)%3;
int vertex_index_2 = (e->second+2)%3;
```
- At this point, it's possible to get the indexes of the vertices at the triangulation level (not inside the face):
```
int node_id_1 = face->vertex(vertex_index_1)->info();
int node_id_2 = face->vertex(vertex_index_2)->info();
```

All the edges are saved in a vector with their squared length. In addition, some of the edges are saved in another vector if they are covered by the current power. A person at distance $d$ from a jammer is safe if $d^2 \leq p/4$. In the limit case, $d = l/2$ where $l$ is the length of the edge, so an edge is safe if $l^2 \leq p$.

To find which mission can be done with power $p$, it's necessary to find the connected components of the graph composed by the edges covered with the given power. Then, for each mission, it's necessary to check if the starting and ending positions of that mission are covered by their closest vertex in the triangulation (as above, a position is safe if $d^2 \leq p/4$). If both the start and the end are safe, and they are in the same connected component in the safe graph, then the mission can be done.

To find the minimum power to do all the missions, it's necessary to order all the edges by increasing squared length and create disjointed sets with storage. For every mission, it's necessary to update the minimum power to allow the starting and ending positions to be under a jammer ($power = 4 \cdot d^2$). Then, the disjointed sets are updated to make it possible to connect the jammers closest to the start and ending positions of the current mission, meaning that they are in the same connected component. For each new edge, it's necessary to update the power needed to allow that edge to be covered. In this newly created "graph" (with the minimum amount of covered edges to allow for completing all missions), there would be useless edges, but it's not a problem because the last added edge will be needed, and it will be the one that will dictate the amount of power needed.

To find the minimum power to do all the missions that can be done with $p$ power, it's possible to do the same thing that has been done when finding the minimum power to cover all missions, but only if a mission can be done with $p$ power.