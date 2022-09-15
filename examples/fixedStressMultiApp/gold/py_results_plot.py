import pandas as pd
import os
import sys
import numpy as np
import matplotlib.pyplot as plt

split = pd.read_csv("~/projects/falcon/examples/fixedStressMultiApp/gold/mandel_master.csv")
couple = pd.read_csv("~/projects/falcon/examples/fixedStressMultiApp/gold/mandel_fully_saturated.csv")

plt.figure(figsize=(9, 3))
plt.subplot(1, 3, 1)
plt.plot(split.time,split.xdisp,'b-',linewidth=1.5)
plt.plot(couple.time,couple.xdisp,'r+',linewidth=2.5)
plt.title('Horizontal displacement')
plt.ylabel("Displacement")
plt.xlabel("time")
plt.grid()

plt.subplot(1, 3, 2)
plt.plot(split.time,split.ydisp,'b-',linewidth=1.5,label = 'Fixed stress split')
plt.plot(couple.time,couple.ydisp,'r+',linewidth=2.5,label = 'Fully coupled')
plt.legend(loc="upper right", prop={'size': 9})
plt.title('Vertical displacement')
plt.xlabel("time")
plt.grid()

plt.subplot(1, 3, 3)
plt.plot(split.time,split.p0,'b-',linewidth=1.5,label = 'Split: x=0')
plt.plot(split.time,split.p5,'r-',linewidth=1.5,label = 'Split: x=0.5')
plt.plot(split.time,split.p8,'k-',linewidth=1.5,label = 'Split: x=0.9')
plt.plot(couple.time,couple.p0,'b+',linewidth=2.5,label = 'Couple: x=0')
plt.plot(couple.time,couple.p5,'r+',linewidth=2.5,label = 'Couple: x=0.5')
plt.plot(couple.time,couple.p8,'k+',linewidth=2.5,label = 'Couple: x=0.9')
plt.legend(loc="upper right", prop={'size': 9})
plt.ylabel('Pore pressure')
plt.xlabel("time")

plt.grid()
plt.tight_layout()
plt.savefig("mandel_results.png",facecolor='w', edgecolor='w')
plt.show()
