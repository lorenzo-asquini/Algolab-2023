# Diet - solution

For each nutrient, there are two inequalities. Considering $q_j$ the quantity of food $j$ to use and $c_{i, j}$ the quantity of the nutrient $i$ that the food $j$ gives, the inequalities are:
- $q_1 \cdot c_{i,1} + ... + q_j \cdot c_{i, j} \leq max_i$
- $-q_1 \cdot c_{i,1} - ... - q_j \cdot c_{i, j} \leq -min_i$. All the inequalities must have the same verse, so it's necessary to change the original verse of this inequality by multiplying it by $(-1)$.

The objective function, considering $p_j$ the price of the food $j$ is $q_1 \cdot p_1 + ... + q_j \cdot p_j$, that needs to be minimized.