# Deck of Cards - solution

Considering that the goal is to have the sum closest to $k$, if the current sum is too high, it's necessary to consider one less element. If it is too low, it's necessary to consider one more element.
It is therefore necessary to use the sliding window technique, paying attention to the indices when near the end. Every time a new sum is calculated, it is necessary to see if it is the best choice of indices $i$ and $j$.