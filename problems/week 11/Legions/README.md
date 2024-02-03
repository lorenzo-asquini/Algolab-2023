# Legions - solution

The goal is to maximize the time needed to reach the target point while, at the same time, making sure that the starting point and the target point are on the same side of each line.

It's necessary to create a LP solver with variables $p_x$ and $p_y$ to describe the target position and a variable $t$ to describe the time needed to reach that point, starting from each line. The main expression can be written as $t \cdot v \cdot \sqrt{a^2+b^2} = |a \cdot p_x + b \cdot p_y + c|$.

For each line of soldiers, calculate if the starting point $s$ is on the left or right of that line. It's just necessary to check the sign of $a \cdot s_x + b \cdot s_y + c$. If the sign is positive, it's possible to remove the absolute value without changing the sign. Considering that it's necessary to have an upper limit on the time because it needs to be maximized, the inequality is $t \cdot v \cdot \sqrt{a^2+b^2} \leq a \cdot p_x + b \cdot p_y + c$ \rightarrow $t \cdot v \cdot \sqrt{a^2+b^2} - a \cdot p_x - b \cdot p_y \leq c$. If the value is negative, it's necessary to change the signs inside the absolute value.

$t$ needs to be positive, and the objective equation has the goal of minimizing $-t$.