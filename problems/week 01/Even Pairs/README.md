# Even Pairs - solution

In order to minimize the number of loops necessary to find the solution, add each new value to a partial sum, and for each new value, increase the counter of the even partial sums or the odd partial sums.

The final formula is therefore: `(even*(even-1))/2 + (odd*(odd-1))/2 + even`