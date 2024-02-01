# Even Matrices - solution

It is easier to create a matrix with dimensions $(n+2) \times (n+2)$ to not have to deal with many edge cases. In this case, when a border element is accessed, adding or subtracting a zero does not make any difference.

Firstly, it is necessary to save in the cell $(i,j)$ the partial sum of the sub-matrix $(0,i,0,j)$. After, it is possible to find the partial sums of the sub-matrices  $(i1, i2, 0, j)$ through difference of partial sums $[(0, i2, 0, j) - (0, i1, 0, j)]$. At this point, for every pair $(i1, i2)$, it is applied the solution of _[Even Pairs](https://github.com/lorenzo-asquini/Algolab-2023/tree/main/problems/week%2001/Even%20Pairs)_.