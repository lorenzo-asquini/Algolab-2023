# Schneewittchen - solution

While reading the input, it's necessary to associate with the _mine index_ (from $0$ to $n-1$) of the dangerous mines a _dangerous mine index_ (from $0$ to $\text{nr\_dangerous\_mines}-1$). This allows later to have variables in the LP problem with a lower identifier. The graph must be constructed backward compared to what is given to allow going down the mines.

Firstly, it's necessary to find all the downstream mines, given the entrance or a dangerous mine. A downstream mine of $x$ is a mine that can be reached using DFS starting from $x$ until another dangerous mine or the end of a path is reached. If another dangerous mine is reached, that mine is still considered one of the downstream mines of $x$. The other dangerous mine encountered is considered because it is used to wrap up in a single value all the resources collected in its downstream mines.

In the LP solver, the variables are:
- A variable for each dangerous mine, for each mineral. Each of these variables contains the amount of mineral $j$ that goes out of a specific dangerous mine after halving.
- A variable for each mineral for the entrance.
- A variable to represent the amount of mineral $j$ that needs to be bought.

To create the inequalities:
- For each dangerous mine or the entrance $x$, iterate over all the minerals:
  - For each downstream mine of $x$, if it's not dangerous, add up the mineral $j$ it contains. If it's dangerous, consider the mineral that goes out of it (a variable) with a $-1$ coefficient.
  - If $x$ is dangerous, consider its variable linked to the mineral $j$ with a coefficient of $2$ because the inequality checks correctness before halving. Otherwise (entrance), the coefficient is $1$.
  - The sum of resources must be less than the sum of the minerals gathered downstream. The inequality is $\text{in\_mat} \leq \text{downstream\_mat} + \text{from\_other\_dangerous}$. This means that it's not possible to have more materials going into a dangerous mine than there are materials downstream.
  - If the mine is not the entrance, limit the in-going materials (so it's necessary to use $2$ as a coefficient) using the danger threshold.

For each material:
  - The amount of minerals bought must be less than the supply.
  - The amount of minerals gathered and purchased must be higher than the amount required.