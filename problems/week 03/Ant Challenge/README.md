# Ant Challenge - solution

Each connection between two trees may be taken by any species, meaning that, at the end, it does not matter who is doing the work.

The given input edges with the relative weights are **ALL** the possible edges that can be used by the insects of each species, because there is always a way, even if long, for an insect to travel between two connected trees. It is necessary to find the minimum scanning trees of each of these graphs to find the different fast networks of each species (this is how the networks were created). At the end, all the edges of the fastest network of each species can be added to a single graph, and then Dijkstra is used. 