# Phantom Menace - solution

Represent each vertex with two vertices. For each edge, connect the odd part of the source vertex to the even part of the target vertex. Connect the source of the MaxFlow to the even parts of the vertices of the starting planets. Connect the odd parts of the vertices of the destination planets to the sink of the MaxFlow. For each vertex, connect its even part to its odd part.

Find the maximum flow that will be equal to the minimum cut. By representing each vertex with an edge, the minimum cut will go through the minimum number of vertices that need to be defended.