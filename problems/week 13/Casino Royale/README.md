# Casino Royale - solution

Connect all the stations with a capacity equal to the maximum number of people allowed in a train and a cost that is the maximum priority. This is necessary to carry on unused flow from one station to another. It's not possible to use $0$ as the cost because, otherwise, it would be better to not do missions.

Connect the start and end stations of each mission with a flow of $1$. The cost is the cost shift (maximum priority) multiplied by all the stations skipped, minus the priority of that mission. Considering that the final function finds the minimum cost, doing a mission removes $q$ from the final cost over not doing a mission.

To adjust the result, it's necessary to remove the found cost from $l \cdot (n-1) \cdot \text{cost\_shift}$, that is, the cost of using the empty trains.