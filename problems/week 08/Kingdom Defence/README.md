# Kingdom Defence - solution

Given the number of soldiers present and needed at a location, it's possible to find the demand and supply for each location (supply stored positively and demand stored negatively). Each path (the edge of a max flow graph) has capacity equal to the difference between the maximum and the minimum capacity of that path. The source location of the path has an increase in demand equal to the minimum capacity. The destination location of the path has an increase in supply equal to the minimum capacity.

All the locations with positive supply are connected to the source, and all the ones with negative supply (demand) are connected to the sink. It's necessary to count the total demand to see if the flow is equal to that value.