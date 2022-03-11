#!/usr/bin/env python3
#* This file is part of the MOOSE framework
#* https://www.mooseframework.org
#*
#* All rights reserved, see COPYRIGHT for full restrictions
#* https://github.com/idaholab/moose/blob/master/COPYRIGHT
#*
#* Licensed under LGPL 2.1, please see LICENSE for details
#* https://www.gnu.org/licenses/lgpl-2.1.html
import pandas as pd
import os
import sys
import numpy as np
import matplotlib.pyplot as plt
import math

#-------------------------------------------------------------------------------
df = pd.read_csv('mrect1_out_fracture_app0.csv')
print('fracture dataframe description: ',df.columns.values)
['time' 'J_out' 'P_in' 'P_out' 'TK_in' 'TK_out' 'kg_out' 'kg_per_s']
df_amr = pd.read_csv('mrect1_amr_out_fracture_app0.csv')
#analytic solution from Koenradd and Gringarten paper, 1975
timevector = np.logspace(1, 9, 50, base=10)
print(timevector)

#analytic Gringarten solution 1975 from Beckers, Koenraad <Koenraad.Beckers@nrel.gov>
Trock = 363-273.15
Tinj = 303-273.15
rhowater = 1000
cpwater = 4150
fracnumb = 1
fracwidth = 100
fracsep = 1000
krock = 2.83
fracheight = 10
rhorock = 2875
cprock = 825
mtot = 25/25/10
Q = mtot/rhowater/fracnumb/fracwidth
td = (rhowater*cpwater)*(rhowater*cpwater)/(4*krock*rhorock*cprock)*(Q/fracheight)*(Q/fracheight)*timevector
Twater=[]
for i in range(len(timevector)):
    temp=1-math.erf(1/(2*math.sqrt(td[i])))
    Twater.append(Trock-temp*(Trock-Tinj))


#-------------------------------------------------------------------------------
fig1=plt.figure()
plt.plot(timevector/3600/24/365,Twater,'b', linestyle = '-',marker = 'o',fillstyle='none',label='Gringarten 1975')
plt.plot(df['time']/3600/24/365,df['TK_out']-273.15,'r', linestyle = '--',label='Falcon')
plt.plot(df_amr['time']/3600/24/365,df_amr['TK_out']-273.15,'g', linestyle = '-.',label='Falcon amr')
plt.ylim([50, 100])
plt.xlim([0, 10])
plt.ylabel("T (degC)")
plt.xlabel("Time (year)")
plt.grid()
plt.legend()
plt.title("production temperature")

plt.savefig("compareGringarten.pdf")

plt.show()
