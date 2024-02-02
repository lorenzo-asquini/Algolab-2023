# Algocoon - solution

The solution can be found by finding the minimum cut, dividing nodes from $0$ to $i-1$ in one part and nodes from $i$ to $n-1$ in the other. In the minimum cut, it's necessary to count only the costs of the edges in one of the two directions, not both. In this case, the solution may come from either one of the two directions (the one with the smallest minimum cut is the correct one), meaning that it's necessary to calculate two maximum flows for each possible division.

To find the minimum cut, it's necessary to calculate the maximum flow. Usually, in an ($S$-$T$)-cut, the source is in $S$ and the sink is in $T$. However, it's not necessary to have a specific source or sink; it's just necessary to pick a node in $S$ to make it the source and a node in $T$ to make it the sink. It's therefore possible to pick node 0 as the source in the subset S and iteratively pick node $i$ as the sink in the subset $T$. It's necessary to consider both the ($S$-$T$)-cut and the ($T$-$S$)-cut (so with source and sink swapped) because it may be possible to choose either of the two parts.

It's not a problem if the source has ingoing edges and/or the sink has outgoing edges.