# Pied Piper - solution

It's necessary to create a DP table `dp[i][j]` with dimensions $n \times n$ (where $n$ is the number of squares). `dp[i][j]` contains the highest number of rats that can be gathered going from `0` to `i` and going back from `j` to `0`. The final answer will be `dp[n-1][n-1]`. It's necessary to populate the table, considering all the squares `dp[i][0]` that are reachable from square `0` (outgoing edges) and all the squares `dp[0][j]` that can reach square `0` (incoming edges). All the cells with the default value will be skipped because they will be considered unreachable on any possible path.

The cell `dp[i][j]` will be considered the starting cell to calculate the values after some movement. If the value of `dp[i][j]` is not the default value (meaning that it is usable forward and/or backward):
- For all the outgoing neighbors of `i` in the graph, if the landing square `k` is such that $k > j \; || \; k == n-1$, `dp[k][j] = max(dp[k][j], dp[i][j] + weight(i -> k)`.
- For all the outgoing neighbors of `j` in the graph, if the landing square `k` is such that $k > i \; || \; k == n-1$, `dp[i][k] = max(dp[i][k], dp[i][j] + weight(k -> j)`.
The value update considers if it's better to use a path considered previously or if it's better to use the newly considered edge.

One important thing to remember is that it's not possible to use a square both for a forward path and a backward path. For this reason, it's necessary to make sure that $k > j$ going forward and $k > i$ going backward. This ensures that $k$ was not considered yet when computing the best number of rats that can be captured in any other path.