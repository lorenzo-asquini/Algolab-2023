# Inball - solution

The distance of the center point of a ball $x = (x_1, x_2, ..., x_d)$ to a plane is $D = \frac{|a_1 \cdot x_1 + a_2 \cdot x_2 + ... + a_d \cdot x_d - b|}{\sqrt{{a_1}^2 + ... + {a_d}^2}}$. This formula must be true for all the inequalities given by the problem that describe a plane in $d$ dimensions.

Considering that $a_1 \cdot x_1 + a_2 \cdot x_2 + ... + a_d \cdot x_d \leq b$, the numerator is always negative, so it's always necessary to multiply it by $(-1)$ to make sure the absolute value gives a positive result.

It's possible to transform the equality that defines the distance of a point to a plane into two inequalities. Considering that it's necessary to maximize $D$, it's possible to choose only the one that limits its positive value: $\sqrt{{a_1}^2 + ... + {a_d}^2} \cdot D \leq -a_1 \cdot x_1 - a_2 \cdot x_2 - ... - a_d \cdot x_d + b$.

$D$ cannot be negative, so it's necessary to give it a lower bound with `lp.set_l(D, true, 0);`. The objective function has the goal of maximizing $D$, so it's necessary to minimize its opposite.