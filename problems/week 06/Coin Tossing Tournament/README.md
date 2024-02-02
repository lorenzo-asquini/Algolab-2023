# Coin Tossing Tournament - solution

Create a graph that can be used to calculate the max flow with nodes for the rounds and for the players.

From the source to each round, the edges have capacity 1 (one point can be assigned per round). From each round to the players, if it's known who will win, there is an edge from the round to the specific player with capacity 1. If it's not known, there is an edge from the round to both players with capacity 1. From each player to the sink, there is an edge with capacity equal to the player's score on the scoreboard.

The scoreboard is plausible if the maximum flow is equal to the number of matches and if the total score on the scoreboard is equal to the number of matches (there are some test cases with more points than matches).