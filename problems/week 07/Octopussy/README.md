# Octopussy - solution

## Recursive method

Each bomb is stored in a custom struct that contains its index (to be able to determine quickly the bombs that are under it), if it was already defused, and the time at which it will explode. At first, it's necessary to store in an ordered vector (determined recursively) all the bombs that it's necessary to defuse before defusing the current bomb (each bomb has such a vector that can contain bombs of multiple levels below). This vector is the ordered union of the two vectors of the left and right bottom bombs. Each bomb stores the union of the vectors of the two bottom bombs and returns this union plus itself to the caller bomb.

After ordering the returned vector when calling the recursive function described above from the top (from the bomb with index 0), there will be a vector containing the bombs in the order in which they need to be defused. For each bomb in the order of defusal found above (defuse bombs that explode sooner first), the bomb is defused with a recursive function. If it has already been defused, nothing happens. If it was not defused, the bombs below are defused recursively, meaning that if a bomb below sits on top of an undefused bomb, it calls the function for that bomb, and so on. These undefused bombs are defused in the order found in the function described above. When all the bombs below are defused, it's possible to defuse the current bomb if there is enough time. If there is not enough time, an explosion flag is set. After defusing a bomb, time is increased by one.

If the explosion flag is not set, all the bombs can be defused.

## Fast method

For all the bombs that lie on top of other bombs, set the defusal time of the two bombs below to be the maximum time at which they can be defused to avoid an explosion. This time is the minimum between the below bomb's defusal time and the defusal time of the bomb above, minus one, indicating that it's possible to defuse the bomb at a maximum of a second before defusing the bomb above.

After ordering the newly defined defusal times, it's necessary to see if each bomb can be defused before the maximum time at which it does trigger an explosion.