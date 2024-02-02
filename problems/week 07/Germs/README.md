# Germs - solution

The first goal is to calculate the time at which each germ will die:
- Try all the dish borders. It's not necessary to check for a collision between a disk and a segment (it would be too expensive); it's possible to just consider the distance between a germ and the dish borders, trying all four combinations of the coordinates of the point and the x-y coordinates of the dish borders. This distance needs to be squared.
- Try all the close pairs of germs defined by the edges of the triangulation created using the germ coordinates. Calculate the squared distance by calculating the squared length of the segment that connects the two points, and then divide it by 4 (death if $distance \leq 2 \cdot r$).

At this point, order all the minimum squared death radii that are stored in a vector. Considering that $t = \sqrt{\sqrt{r^2}-0.5}$, it's possible to calculate the requested timings by getting the time of the first element, the element at 'n/2', and the last element in the minimum squared death radius vector. The results need to be transformed into a long with a ceiling.