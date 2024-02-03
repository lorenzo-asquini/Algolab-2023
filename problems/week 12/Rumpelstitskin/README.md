# Rumpelstitskin - solution

It's not possible to have the two networks meet on a single island because it's not possible to have a flow out of an island equal to exactly 0 or 2 (without it being 1). For this reason, it's necessary to have a flow from one network to the other, going through the islands.

The source is connected to the castle of network A with a certain amount of sacks usable, and then network A is constructed. The islands are represented using two nodes. Network A connects to the even node. There is an edge of capacity 1 (each island can be used only once) that connects the two nodes of each island. Each odd node of the islands is connected to the right node in network B. Network B is represented in reverse, and its castle is connected to the sink.

The preferences are represented with an offset because they need to be represented as a negative number. This means that, at the end, it's necessary to adjust the result.