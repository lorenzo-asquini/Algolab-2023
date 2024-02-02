# Real Estate Market - solution

In a graph of size `S+M+N`:
- Connect the source to each state. The edges should have a capacity equal to the number of properties that can be sold in that state (so there is a limit on how many properties can be sold in each state). The cost should be 0.
- Connect each state to all the properties that are present in that state, with edges of capacity 1 and cost 0.
- Connect each property to each buyer with capacity 1 (each buyer can buy a property only once) and a cost of `100 - bid`. This is necessary because, ideally, the profit should be the opposite of the cost from the ALGO point of view. However, to use the efficient algorithm, the cost shouldn't be negative.
- Connect each buyer to the sink with capacity 1 (because each buyer wants to buy at most one property) and cost 0.

At the end, the number of properties sold is equal to the maximum flow, and the profit is $100 \cdot PropertiesSold - MinimumCost$, this because $MinimumCost = 100 \cdot PropertiesSold - Profit$.