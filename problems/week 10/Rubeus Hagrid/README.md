# Rubeus Hagrid - solution

Create a custom struct that contains the best number of galleons acquired starting from a chamber, the amount of time it took to do the whole outgoing trip starting from that chamber, and the number of chambers encountered (considering also the current chamber).

Recursively:
- Base case: if a chamber has no outgoing tunnels, return the number of galleons in the current cave, 0 as the time needed to traverse all outgoing tunnels (there aren't any), and 1 as the number of chambers traversed (this one).
- Otherwise, for each outgoing tunnel:
  - Call the function recursively and get some statistics. Add to a list the statistics of the path just taken. The number of traversed nodes remains the same (the current cave is not yet considered), and the time needed to return increases by 2 times the time needed to traverse the outgoing tunnel. It's also necessary to adjust the galleons, considering that galleons in each cave disappeared by the amount of time it took to traverse the outgoing tunnel.
  - Order the different paths by the number of galleons lost by taking another path. For example, by taking `p1` and then `p2`, it would be necessary to remove from the galleons in each cave in `p2` those that disappeared while traversing `p1`.
  - For each path in the possible paths:
	- Increase the total amount of traversed chambers.
	- Add to the galleons found in the current cave the galleons found in the path minus the number of galleons that disappeared while traversing other paths.
	- Increase the time needed to traverse the caves in the outgoing trip.