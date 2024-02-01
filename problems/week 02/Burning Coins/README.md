# Burning Coins - solution

Each player (me and the rich guy) can do one of two things: take from the left or take from the right of the line of coins. During the game, the same configuration described by two indices $i$ and $j$ may present itself after different combinations of moves, but the pairs of indices $(i, j)$ are unique to each player. This means that, no matter the moves, a given pair $(i, j)$ always appears to the same player. This makes it possible to use only one DP table.

For every new move, the main guy (me) needs to take the coin that, summed to the best result possible given the remaining coins, gives the highest value. At the same time, the rich guy wants the main player to make as little money as possible, so he wants to take the coin that gives me the lowest value.

- If it's the main guy's turn (my turn), given that the remaining coins have indices from $i$ to $j$ included, the formula to determine the maximum final money is: $money_{max} = \max(v_i + m[i+1][j], v_j + m[i][j-1])$. If $i = j$ (only one coin remains), then $m[i][j] = v_i$.
- If it's the rich guy's turn, given that the remaining coins have indices from $i$ to $j$ included, the formula to determine the maximum final money is: $money_{max} = \min(m[i+1][j], m[i][j-1])$. If $i = j$ (only one coin remains), then $m[i][j] = 0$.