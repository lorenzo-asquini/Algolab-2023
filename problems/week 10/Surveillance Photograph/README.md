# Surveillance Photograph - solution

Divide each vertex into two vertices in a BGL graph for max flow. Then, connect the source to the even vertices of the police stations and the odd vertices of the police stations to the sink (capacity is 1); connect each even vertex of an intersection where there is a photograph to its paired odd vertex (capacity is 1). This also handles cases where there are photographs and police stations at the same intersection.

For each road, connect the even vertices of each intersection with infinite capacity. This creates the network for moving between the intersections. Connect the odd vertices of each intersection with capacity 1. This creates the network where photos are transported. When a photo is picked up, it's necessary to move from the even network to the odd network, allowing for all the requirements to be met.