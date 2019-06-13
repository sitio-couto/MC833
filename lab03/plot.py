#!/usr/bin/env python
# a bar plot with errorbars
import numpy as np
import matplotlib.pyplot as plt

op_means = (4.260997, 3.735667, 3.847504, 5.376635, 4.403842, 7.504373)
op_std =   (0.616119, 0.243934, 0.390795, 1.556925, 0.267154, 5.517604)

com_means = (1.015860, 1.060541, 1.083282, 1.142348, 1.447465, 1.373099)
com_std =   (0.100033, 0.077996, 0.086935, 0.122500, 0.120674, 0.107703)


ind = np.arange(len(op_means))  # the x locations for the groups
width = 0.35  # the width of the bars

fig, ax = plt.subplots()

rects1 = ax.bar(ind - width/2, op_means, width, yerr=op_std,
                color='SkyBlue', label='Consulta/Atualização')
rects2 = ax.bar(ind + width/2, com_means, width, yerr=com_std,
                color='IndianRed', label='Comunicação')

ax.set_ylabel('Tempo (ms)')
ax.set_title('Tempo de consulta/atualização e de comunicação')
ax.set_xticks(ind+(width/2))
ax.set_xticklabels( ('Op1', 'Op2', 'Op3', 'Op4', 'Op5', 'Op6') )
ax.legend(loc='upper left')

def autolabel(rects, xpos='center'):

    xpos = xpos.lower()  # normalize the case of the parameter
    ha = {'center': 'center', 'right': 'left', 'left': 'right'}
    offset = {'center': 0.5, 'right': 0.57, 'left': 0.43}  # x_txt = x + w*off

    for rect in rects:
        height = rect.get_height()
        ax.text(rect.get_x() + rect.get_width()*offset[xpos], 1.01*height,
                '%.1f'%float(height), ha=ha[xpos], va='bottom')


autolabel(rects1, "left")
autolabel(rects2, "right")
plt.tight_layout()  
plt.show()
