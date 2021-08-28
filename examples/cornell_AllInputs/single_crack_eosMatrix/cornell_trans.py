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


inputs=['0']
labels = {inputs[0]: 'current'}
colours = {inputs[0]: 'g'}

plt.figure()

df = pd.read_csv('cornell_trans_out_fracture_app0.csv')
dfBinInitial = df[df['P_out'] < 30.6]
dfBinProduction = df[df['P_out'] > 30.6]
plt.plot(dfBinInitial['time'] / 3600.0, dfBinInitial['TK_out'] - 273,\
         'b' + ":")
plt.plot(dfBinInitial['time'].iloc[-1] / 3600.0, dfBinInitial['TK_out'].iloc[-1] - 273,\
         'b' + "o")
plt.plot(dfBinProduction['time'] / 3600.0, dfBinProduction['TK_out'] - 273,\
         'b', label='current')
plt.grid()
plt.legend()
plt.title("Production temperature: with heat transfer, various mesh sizes")
plt.xlim([0, 20])
plt.ylim([60, 100])
plt.xlabel("time (hours)")
plt.ylabel("T (degC)")
plt.savefig("matrix_app_T_short.png")
plt.show()
plt.close()

plt.figure()
plt.plot(dfBinProduction['time'] / 3600.0, dfBinProduction['TK_out'] - 273,\
         'b', label='current')
plt.grid()
plt.legend()
plt.title("Production temperature: with heat transfer, various mesh sizes")
plt.xlim([0, 740])
plt.xlabel("time (days)")
plt.ylabel("T (degC)")
plt.savefig("matrix_app_T.png")
plt.show()
plt.close()

sys.exit(0)
