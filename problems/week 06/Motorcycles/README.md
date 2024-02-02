# Motorcycles - solution

The most intuitive solution would require calculating the intersections of each pair of bikers (if any), then finding the biker that reaches the intersection point first. However, this is not fast enough for the last test sets.

For each biker, it's necessary to store its index, the $y$ coordinate of its starting position, and the slope of its path (stored using `EPEC:FT`, it's necessary to have infinite precision, and a `double`, which is the same thing as using `EPIC::FT`, is not enough). Then, the bikers are sorted by their $y$ starting position, with the highest values first.

After creating an array that will contain all the bikers that will continue forever, for every biker, it's necessary to start a loop:
- If the array is empty or if the slope of the current path is lower than the slope of the last forever path, there is no collision. It's possible to add the current path to the list of forever ones (_break_).
- If there is a collision (the slope of the current path is higher than the slope of the last forever path):
  - If the slopes have the same sign and this sign is positive, this means that the bottom path is steeper and it encounters the path of the previous motorcyclist, so the current path can be ignored (_break_).
  - If the slopes have the same sign and this sign is negative, this means that the bottom path is less steep, and so the previous motorcyclist will encounter this path. It's necessary to remove the previous path from the forever ones and continue the search for collisions (_continue_).
  - If the slopes don't have the same sign and the absolute value of the slope of the current path is lower than or equal to (to consider also the right first rule) the absolute value of the previous forever path, it's necessary to remove the previous path from the forever ones and continue the search for collisions (_continue_).
  - If the slopes don't have the same sign and the absolute value of the slope of the current path is greater than the absolute value of the previous forever path, the current path can be ignored (_break_).

At the end, it's necessary to print the indexes of the forever bikers, considering the original order.