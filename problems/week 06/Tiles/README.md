# Tiles - solution
 
If the number of spaces to tile is odd, then it's not possible to tile the area.

Prepare a graph for max flow. For every space, if it's an `x`, skip that space. If not:
- If the space is "even", meaning that `i+j` is even, make that space the starting position for a tile. Connect it to the source with an edge of capacity one, meaning that it's possible to choose only one of the possible tiles that start at that space. Try to place the four tiles that start with that space, connecting that space to the other space covered by the tile considered.
- If the space is "odd", that means it's a landing space for a tile. Connect the space to the sink with capacity 1, meaning that each space can be covered once.

At the end, the max flow must be equal to half the number of spaces that need to be tiled (only the landing spaces are considered in the flow).

Pay attention to the fact that it's necessary to consider the starting spaces in a checkerboard manner to make it possible to logically divide the starting and landing spaces. This allows for well-defined edges and capacities. Making it possible for a space to be both a starting space and a landing space does not work.