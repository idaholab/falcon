import pandas as pd
import os
import sys
import numpy as np
import matplotlib.pyplot as plt

df = pd.read_csv('forge_native_point_data_56_32_0001.csv')
plt.figure(figsize=(9, 5))
plt.subplot(1, 3, 1)
plt.plot(df.pressure/1E6,df.z,'b-',linewidth=1.5)
plt.title('Well-56-32')
plt.ylim([-1000, 1800])
plt.ylabel("Well Vertical Coordinate, m")
plt.xlabel("Pore Pressure, MPa")
plt.grid()
plt.subplot(1, 3, 2)
plt.plot(df.temperature-273.15,df.z,'b-',linewidth=1.5)
# plt.legend(loc="upper right", prop={'size': 9})

# plt.xlim([0, 20])
plt.ylim([-1000, 1800])
# plt.ylabel("Well Vertical Coordinate, m")
plt.xlabel(r"Temperature, $^oC$")
plt.grid()
plt.subplot(1, 3, 3)
plt.plot((df.stress_ii+df.stress_jj)/2/1e6,df.z,'g:',linewidth= 2.5, label = r"$\sigma_h$")
plt.plot(df.stress_kk/1e6,df.z,'b-',linewidth=1.5,label = r"$\sigma_v$")
plt.legend(loc="upper left", prop={'size': 9})
plt.ylim([-1000, 1800])
# plt.ylabel("Well Vertical Coordinate, m")
plt.xlabel("Stress, MPa")
plt.grid()
plt.tight_layout()
plt.savefig("well_56_32_demo.png",facecolor='w', edgecolor='w')
plt.show()
