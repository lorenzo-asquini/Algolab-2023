# First hit - solution

To reduce the number of constructions to make, it's better to check if an intersection between the ray and the currently considered segment even exists. In the event that the intersection between the ray and a segment is the segment itself, it's necessary to check which one of the two points of the segment is closer to the starting point of the ray.

Always using the starting ray would be too slow for the last test set. This is because, even if a closer intersection point is found, the ray would continue to consider the segments farther away, although not necessary (when a far intersection is found, the exact intersection point would be calculated even if useless). For this reason, after the first intersection is found, the ray can be substituted by a segment starting from the ray origin and ending at the closest intersection found. This segment can be used in place of the ray in all the various parts of the program. It is necessary to use a boolean to see if the ray or the replacement segment should be used. The replacement segment is available only after the first intersection.

Another thing to consider is that if the input is ordered, placing first the segments farther away, all the segment collision points should be calculated. To avoid this, it is better to shuffle the input using `std::random_shuffle()`.