# New York - solution

The problem can be solved using a sliding window on a tree. It's necessary to have a `deque` to store the current path, a `multiset` to store all the temperatures of the current path, and a `vector` or a `set` to store the valid starting points.

Recursively:
- Add a new point to the path and consider its temperature.
- If the path is too long, remove (and store) the first point in the path. To do so, it's necessary to use `erase(iterator)` and not `erase(value)` because the latter erases all the occurrences (to find the correct iterator, it's possible to use the method `find(value)`).
- If the new length is correct, get in $O(1)$ the minimum and maximum temperatures using the multiset, and check if it's necessary to add a valid starting point to the list of valid starting points.
- Call the function recursively for all the children of the last point in the path.
- Restore the previous status of the path. Remove the newly inserted point (last point) and, if there was a deletion, add back the deleted point at the front (with its temperature).