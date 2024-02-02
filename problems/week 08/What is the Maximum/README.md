# What is the Maximum - solution

To improve readability, it's better to give a name to the variable indexes, for example, `const int X = 0`.

The solver always minimizes the objective function. It's necessary to invert all the signs in the objective function to maximize it, then it's necessary to invert the sign of the objective value to have the right result.