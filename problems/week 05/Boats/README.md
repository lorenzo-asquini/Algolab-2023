# Boats - solution

Order the boats by their ring position, then proceed with a greedy algorithm with a little twist.

Take the first boat and set the position of its ring as the rightmost occupied position. Then, for each boat:
- Try to place the new boat so that it occupies only the space until its ring (this results in less space being occupied). To do this, it's necessary to see if, by placing the boat in this position, it would overlap with the occupied positions of other boats. Increase the number of boats used if this positioning is possible.
- If the previous attempt was not successful, try to place the boat in a less optimal way by placing it right next to the previous boat and terminating farther than its ring. This is possible if the boat's ring doesn't overlap with the occupied positions of other boats. Increase the number of boats used if this positioning is possible.
- If none of the other tries were successful, this means that the boat cannot fit. However, it may be possible to substitute the previous boat with the current one, resulting in less space being occupied. To do so, it's also necessary to store the space occupied without the previous boat to see if, considering the cases described above, the current boat is better than the previous. Do not increase the number of boats used because it's just a substitution.