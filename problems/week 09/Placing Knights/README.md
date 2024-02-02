# Placing Knights - solution

Create a graph of size `n*n` that represents the board.

It's an error to consider each field both as a possible starting position and a possible landing position. Some of them must be considered only as starting positions, and some of them only as landing positions. For this reason, if a field is not a hole, if `(i+j)%2 == 1` the field is a landing position; otherwise, it is a starting position. This is true because, from an "even" starting position, the landing position will always be "odd".

Connect the source to all the starting positions and all the landing positions to the sink. For each starting position, for each of the possible landing positions, connect to it if it's valid (inside the board and not a hole).

Calculate the maximum flow, which is equal to the maximum matching, which is equal to the minimum vertex cover. The maximum independent set is equal to the difference between the number of total non-hole fields and the minimum vertex cover. The solution is the maximum independent set because it must represent the knights that are not connected by two edges that go through a common possible landing position.