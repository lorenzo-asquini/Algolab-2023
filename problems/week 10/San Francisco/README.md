# San Francisco - solution

It's not necessary to create a full BGL graph, but a vector of vectors of integers is enough. After reading the input (pay attention to the fact that the claimed score is too large for an integer), search for the node with no outgoing edges, which is the return hole.

Create a DP table of size $n \times (k+1)$. For each node $i$, it will store the maximum amount of points that can be done starting from that node and doing $j$ moves. The table is initialized by setting $0$ when $j = 0$, meaning that, from any node, without moves, it's not possible to do any point.

Then:
- For each number of moves remaining $j$ from $0$ to $k$:
  - For each node $i$ from $1$ to $n-1$:
    - For each outgoing edge starting from $i$, search for the maximum value:
      - If $i$ is the return node, get the value associated with the node $0$ at the same number of moves remaining.
      - Otherwise, find the highest amount of points that can be done by adding the number of points made going from $i$ to the next node to the number of points that can be done on the arriving node with one move less.

At the end, try all the values stored in `table[0][j]`, trying to see if enough points can be made.