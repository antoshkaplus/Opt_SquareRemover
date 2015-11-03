
import matplotlib.pyplot as plt


stats = map(lambda x: map(float, x.split()), open("stats.txt").read().split("\n")[:-1])
#print len(stats), len(stats[0])
#print stats

(
iteration,
four_count,
pre_four_count,
position_balance,
board_balance,
elim_count)  = map(list, zip(*stats))


#for i in range(1, len(elim_count)):
#    elim_count[i] += elim_count[i-1] 

import numpy as np



    
plt.plot(iteration[500:600], elim_count[500:600],
         iteration[500:600], position_balance[500:600],
         iteration[500:600], board_balance[500:600],
         #x, pre_four_count
         )
plt.show()