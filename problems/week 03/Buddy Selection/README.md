# Buddy Selection - solution

It is necessary to find new connections with more than $f$ common hobbies. There may be different connections for each student, but it is necessary to find at least one possible set of pairs of students that includes all the students.

To quickly check how many common hobbies two students have, it is possible to sort the arrays containing the hobbies and then check one by one the strings, increasing the pointing index in one array or the other (or both) depending on the lexographical order. This brings the computational time for all the comparisons from $O(c^2)$ to $O(c \cdot \log c)$ for every student. If there are enough common hobbies among the two considered students, it's possible to add an edge to the undirected graph.

It is necessary to find the maximum number of edges such that no two edges share any endpoints. This is done using Edmond's algorithm for maximum matching. If the number of edges in the maximum matching is half the number of students, the division done by the director is not optimal.