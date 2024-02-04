# Fighting Pits of Meeren - solution

Store the fighters backwards (starting from $n-1$ to $0$) in order to go through the queue in an easier order. Add 1 to each fighter type because 0 should be reserved for "no fighter".

Create a DP table $n \times 25 \times 25 \times 25$. Given `dp[a][b][c][d]`, `a` is used to denote the current fighter processed, `b` encodes the last two fighters coming from the north, `c` encodes the last two fighters coming from the south, and `d` denotes the difference in the number of entrances between the north gate and the south gate. Consider that:
- There are 5 different fighter types (4 + no fighter). At maximum, a spectator can remember three fighters. Considering that the third one is the newly inserted one, it's possible to just store the previous two by encoding them with $5*x+y$. 
- The excitement cannot be negative. Considering that the maximum excitement for a turn can be only 3000 (last 3 multiplied by 1000), $|p-q| < 12$ to have a positive number. For this reason, it's possible to store the difference using only 25 cells (with an up-shift for negative numbers). Start with empty north and south queues (filled with zeros) of size 3 (worst case).

In the recursion, that returns an integer:
- If $|p-q| \geq 12$, return `INT_MIN` because that combination must not be used (negative excitement).
- If $n = -1$, return 0 because there are no more fighters.
- Find the correct cell in the DP table by encoding the last two fighters from both entrances and shifting the difference. If the value was already determined, return that value. Otherwise, find the excitement in the case the fighter entered through the north gate and the one in the case the fighter entered through the south gate: $distinct \cdot 1000 - 2^{diff}$. For each entrance, check if the excitement is positive. If so, check which one is better by adding to the excitement of the current round the excitement that comes from the recursion of using one less fighter and changing the difference. This value is saved in the DP table.