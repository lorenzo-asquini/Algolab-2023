# Severus snape - solution

The sums may overflow an `int`, so a `long` is needed. In addition, when reading `a` and 'b', it's necessary to read them as `long` because, when multiplied by the number of potions, which is an `int`, the result may overflow an `int` ('int * int` is an `int`).

To consider the A potions, it's necessary to follow a DP approach. The DP table is a three-dimensional array of `long` (let's call it `power`) of size $(n+1) \times (n+1) \times (H+1)$. `power[j][i][h]` denotes the maximum power that is achievable by taking `j` A potions out of the first `i` A potions that result in a happiness of at least `h` (if the happiness is greater than $H$, it's set to $H$). The starting value of each cell should be `LONG_MIN` because `-1` could allow for acceptable values (that are, however, not acceptable).

Firstly, it's necessary to populate the DP table with the value when considering only one of the A potions among the first `i` A potions. For each possible value of `h`, while considering one of the A potions in the first `i`:
- Take the same potion that was taken when considering the first `i-1` potions that grants at least the same happiness, `h`:
- See if it's possible to get a better potion now that it's possible to consider also the potion `i`. If the potion `i` grants at least `h` happiness and increases the amount of power that can be achieved by taking only one potion among the first `i`, consider that potion.

At this point, the array is filled with the best power that can be achieved, considering only one potion among the first `i` A potions that grants at least `h` happiness.
To populate the rest of the DP table, consider iteratively `j` potions at the time out of all possible potions, for each possible value of `h`, considering that the `j` potions are out of `i` total potions (`i` denotes out of how many A potions those `j` potions are considered):
- out how the happiness would be if the previous A potion was not taken. Then try to see if it's better to just skip the current potion or take the current potion instead of the previous one (replacing the previous one means considering the case when `i-1` A potions are considered and `j-1` of them are taken).

At this point, all the cells `power[j][n][H]` contain the maximum power achievable by taking `j` potions while satisfying the happiness `H`. Sort the B potions by their wit, with potions with higher wit first. This is needed because the B potions are chosen using a greedy approach, while the A potions cannot be chosen using a greedy approach because there are two parameters to consider.

Trying to take each B potion one by one, increase the wit by the amount given by that potion. If $W$ is reached, try all the possible values of `j` (the number of A potions taken), making sure that the wit does not decrease too much. If with a certain value of `j` the power is enough to have $P$ power and compensate the loss from the B potions, a valid number of A potions is found.