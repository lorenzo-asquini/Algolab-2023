# Asterix and the Chariot Race - solution

Each city may be in three different states (each associated with a cost): fixed directly; fixed indirectly, meaning that a neighboring city is fixed and the current one is not; or maybe not fixed, meaning that the city is not fixed directly and there is no guarantee that the current city is fixed by a neighboring city.

Recursively, given a city:
- If there are no outgoing paths, the cost to fix the city directly is the one given by the problem; the cost to fix the city indirectly is equal to the cost to fix it directly because otherwise it would be fixed for free; there is no cost associated with a "maybe not fixed".
- If there are outgoing paths, for each outgoing city:
  - Find the best cost to have the outgoing city already fixed (directly or indirectly).
  - Add the cost of having the outgoing city already fixed to a variable in order to know how much it would cost to have all the outgoing cities already fixed.
  - Starting from the cost to fix the current city, add to it the best cost between having the outgoing city already fixed or having the outgoing city maybe broken (it does not matter if the outgoing is broken or not because the current one will fix it).
  - Find out how much it would cost to have the current city fixed by an outgoing city. It's necessary to find, among all outgoing cities, the minimum difference between the outgoing city being fixed directly and the best cost to have the outgoing city fixed in one way or another (found earlier). If the outgoing city was fixed directly, there would be no additional charge. Otherwise, it will be necessary to have the least costly outgoing city fixed directly.
  - The cost to directly fix the current city will already be found. The cost of having the current city indirectly fixed will consider the cost of having all outgoing cities already fixed and the additional cost (that may be zero) of having at least one of the outgoing cities directly fixed. The cost of maybe not being fixed will just be the cost of having all the outgoing cities already fixed, in one way or another.