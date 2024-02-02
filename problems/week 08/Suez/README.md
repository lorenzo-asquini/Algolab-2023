# Suez - solution

In this problem, the input values are fractions, so the input type and the type for the solver should be `CGAL::Gmpq`.

First of all, it's necessary to set the constraints between new posters. It's necessary to consider that:
- $x_1 + \frac{a_1 \cdot w}{2} \leq x_2 -  \frac{a_2 \cdot w}{2}$ → $\frac{w}{2} \cdot (a_1+a_2) \leq x_2 - x_1$ → $a_1+a_2 \leq \frac{2 \cdot (x_2-x_1)}{w}$. The same applies for the y-axis.

To choose the constraint for $a_1+a_2$ it's necessary to get the maximum between $\frac{2 \cdot |x_2-x_1|}{w}$ and $\frac{2 \cdot |y_2-y_1|}{h}$ (the absolute value is needed to consider both directions with a single formula). To understand why the maximum is necessary, it's necessary to think, for example, about the case when $x_1 = x_2$, using the minimum would lead to 0 as the constraint (but the bound should be at least 2).

Then it's necessary to set the constraints between each new poster and the old posters:
- $x_1 + \frac{a_1 \cdot w}{2} \leq x_2 -  \frac{w}{2}$ → $a_1 \leq (x_2 - x_1 - \frac{w}{2}) \cdot \frac{2}{w}$ → $a_1 \leq \frac{2 \cdot (x_2-x_1) - w}{w}$. The same applies for the y-axis.

To choose the constraint for $a_1$, it's necessary to get the maximum between $\frac{2 \cdot |x_2-x_1| - w}{w}$ and $\frac{2 \cdot |y_2-y_1| - h}{h}$ (the absolute value is needed to consider both directions with a single formula) for each old poster while taking the minimum of these constraints across all old posters for each new poster.

The objective function to minimize is $(a_1 - a_2 - … - a_n) \cdot (-2 \cdot(h+w))$.