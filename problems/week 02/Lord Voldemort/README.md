# Lord Voldemort - solution

In the first part of the problem, the sliding window technique is needed because a member can only destroy objects that are in sequence. When the sum inside a window is equal to $k$, the bounding positions should be saved (the number of objects destroyed can be determined from the bounding indices). The window should continue to slide because the best answer may contain windows that overlap with worse windows.

Given $m$ people and $n$ objects, it is necessary to create the memoization array with $m+1$ rows and $n+1$ columns, full of invalid values (for example, $-1$). There are two types of base cases. If there are no members left, no objects can be destroyed. If there are no objects remaining, no objects can be destroyed.

Given a point $(i, j)$ in the array, if a window is skipped or does not exist, the next object is considered, $(i+1, j)$. If it exists and it is not skipped, the number of people that can still do something decreases by one, the window is considered fully (so the next object considered is the one after the end of the window), and the number of objects destroyed increases, $(w[i]+1, j-1)$.

At the end, it is necessary to see if all the members destroyed some objects.