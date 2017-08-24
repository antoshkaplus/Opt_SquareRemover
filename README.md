# SquareRemover
TopCoder marathon competition [TCO14 Round 1](https://community.topcoder.com/longcontest/?module=ViewProblemStatement&rd=15938&pm=13132)

Beam search with score formula:
sq_removed + (1. - pow(rate, sq_move_count)) / (1. - rate_)

sq_removed - squares that already removed
sq_move_count - available combinations that let finish the square in one move
rate - how fast reduce influence of each new combination (manually selected)

Using geometric series sum smoothly reduces influence of each new square combination 
on the grid.

Dividing problem inputs into groups helped to see relationships with different parameters, 
exposes hidden problems of some of them.

Work can be done to optimize the code overall. 
Finding secondary additional score parameter may increase score even farther. 
Find optimal beam width for each input group.

Current solution could get a T-short.

