# Knights - solution

In a graph created in order to find the max flow, each intersection can be modeled using two nodes, one with an even id and one with an odd id. From the even one, it's possible to model the hallways that connect the even nodes to the neighboring odd nodes. These hallway edges have capacity 1 because they can be used only once before collapsing. If a node is next to an exit, the edge connects to the sink (always with capacity one, because it's necessary to go through a hallway to exit). It's necessary to remember that corner intersections have two ways out. The odd node of an intersection is connected to the even node with an edge of capacity C because they represent the intersections.

The knights are edges from the source to the odd nodes of the intersections they are at. The answer is the maximum flow through the graph.