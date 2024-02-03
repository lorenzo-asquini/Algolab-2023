# Clues - solution

Create the triangulation of the stations with vertices that also contain their index information. Using the edge iterator described in _emst.cpp_, save only the edges whose length is less than $r$ (to make the process less heavy, use the squared length and $r^2$).

Using BFS, color the nodes with alternating colors if possible. Maintain an array with a cell for each node that contains $0$ if the node was never visited, $1$, or $2$ to express which frequency was assigned to that node. For each node, check if it already has a frequency assigned. If not, assign the frequency of $1$ to that node and add it to a queue. While the queue is not empty, pop a node from the queue and explore its neighbors:
- If a neighbor was never visited, assign the opposite frequency compared to this one and add it to the queue.
- If a neighbor has the same frequency as the current station, set the interference flag and empty the queue.

At this point, it's necessary to find an interference "over" stations with a different frequency. If the two stations with the same frequency, even if separated by a statio with a different frequency, were closer than $r$, there would still be interference, but it would not be shown immediately because the coloring is valid. By creating a triangulation with only nodes of the same frequency and by checking the length of each edge in these triangulations, it's possible to check for additional interferences.

Finally, if there is any kind of interference, print `n`. If the two mission points are close enough, print `y`. If any of the two points is too far from a station, print `n`. If the two points are on the same connected component, print `y`. Print `n` otherwise.
