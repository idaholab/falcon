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


inputs=['refine0','refine1','refine2','amr1','amr2','refine2_noSubCycle']
labels = {inputs[0]: 'refine0',\
          inputs[1]: 'refine1',\
          inputs[2]: 'refine2',\
          inputs[3]: 'amr1',\
          inputs[4]: 'amr2',\
          inputs[5]: 'refine2_noSub'}
colours = {inputs[0]: 'r',\
           inputs[1]: 'g',\
           inputs[2]: 'b',\
           inputs[3]: 'c',\
           inputs[4]: 'y',\
           inputs[5]: 'k'}
lines = {inputs[0]: '-',\
         inputs[1]: '-',\
         inputs[2]: '-',\
         inputs[3]: '--',\
         inputs[4]: '--',\
         inputs[5]: ':'}
markers = {inputs[0]: '',\
         inputs[1]: '',\
         inputs[2]: '',\
         inputs[3]: '',\
         inputs[4]: '',\
         inputs[5]: ''}

#-------------------------------------------------------------------------------
df = pd.read_csv('refine0/frac.csv')
print('fracture dataframe description: ',df.columns.values)
df = pd.read_csv('refine0/matrix.csv')
print('\n\nmatrix dataframe description: ',df.columns.values)

#-------------------------------------------------------------------------------
#print data
print('iname    nelems    active_time    m_total_nl_its    f_total_nl_its    m_total_steps    f_total_steps')
for iname in inputs:
    df = pd.read_csv(iname+'/matrix.csv')
    active_time=df['active_time'].max()
    nelems=df['nelems'].max()
    m_total_nl_its=df['total_nl_its'].max()
    m_steps = len(df.columns)
    df = pd.read_csv(iname+'/frac.csv')
    f_total_nl_its=df['total_nl_its'].max()
    f_steps = len(df.columns)
    print(iname,', ',nelems,', ',active_time,', ',m_total_nl_its,', ',f_total_nl_its)




#-------------------------------------------------------------------------------
fig1=plt.figure()
for iname in inputs:
    df = pd.read_csv(iname+'/frac.csv')
    dfBinInitial = df[df['P_out'] < 10.6]
    dfBinProduction = df[df['P_out'] > 10.6]
    plt.plot(dfBinInitial['time'] / 3600.0, dfBinInitial['TK_out'] - 273,\
             colours[iname] + ":")
    plt.plot(dfBinInitial['time'].iloc[-1] / 3600.0, dfBinInitial['TK_out'].iloc[-1] - 273,\
             colours[iname] + "o")
    plt.plot(dfBinProduction['time'] / 3600.0, dfBinProduction['TK_out'] - 273,\
             colours[iname], linestyle = lines[iname],marker = markers[iname],\
             fillstyle='none', label=labels[iname])
plt.grid()
plt.legend()
plt.title("Production temperature: with heat transfer, various mesh sizes")
plt.xlim([0, 2])
plt.ylim([195, 205])
plt.xlabel("time (hours)")
plt.ylabel("T (degC)")
plt.savefig("matrix_app_T_short.png")

#-------------------------------------------------------------------------------
fig2=plt.figure()
for iname in inputs:
    df = pd.read_csv(iname+'/frac.csv')
    dfBinInitial = df[df['P_out'] < 10.6]
    dfBinProduction = df[df['P_out'] > 10.6]
    plt.plot(dfBinInitial['time'] / 3600.0/ 24.0, dfBinInitial['TK_out'] - 273,\
             colours[iname] + ":")
    plt.plot(dfBinInitial['time'].iloc[-1] / 3600.0/ 24.0, dfBinInitial['TK_out'].iloc[-1] - 273,\
             colours[iname] + "o")
    plt.plot(dfBinProduction['time'] / 3600.0/ 24.0, dfBinProduction['TK_out'] - 273,\
             colours[iname], linestyle = lines[iname],marker = markers[iname],\
             fillstyle='none', label=labels[iname])
    # plt.plot(dfBinProduction['time'] / 3600.0 / 24.0, dfBinProduction['TK_out'] - 273,\
    #           colours[iname], linestyle = lines[iname],label=labels[iname])
plt.grid()
plt.legend()
plt.title("Production temperature: with heat transfer, various mesh sizes")
plt.xlim([0, 1200])
plt.ylim([120, 205])
plt.xlabel("time (days)")
plt.ylabel("T (degC)")
plt.savefig("matrix_app_T.png")

#-------------------------------------------------------------------------------
fig3=plt.figure()
for iname in inputs:
    df = pd.read_csv(iname+'/matrix.csv')
    plt.plot(df['time'] / 3600.0/ 24.0, df['active_time'],\
             colours[iname], linestyle = lines[iname],marker = markers[iname],\
             fillstyle='none', label=labels[iname])
plt.grid()
plt.legend()
plt.title("Simulation Time")
plt.xlim([0, 1200])
# plt.ylim([120, 205])
plt.xlabel("time (days)")
plt.ylabel("Run Time (s)")
plt.savefig("timing.png")

#-------------------------------------------------------------------------------
fig4=plt.figure()
for iname in inputs:
    df = pd.read_csv(iname+'/matrix.csv')
    plt.plot(df['time'] / 3600.0/ 24.0, df['volume_10deg'],\
             colours[iname], linestyle = lines[iname],marker = markers[iname],\
             fillstyle='none', label=labels[iname])
plt.grid()
plt.legend()
plt.title("Simulation Time")
plt.xlim([0, 1200])
# plt.ylim([8e6, 8.5e6])
plt.xlabel("time (days)")
plt.ylabel("10deg increase volume")
plt.savefig("timing.png")
#-------------------------------------------------------------------------------
fig1 = plt.figure()
ax1 = fig1.add_subplot(1,1,1)
color = 'tab:green'
ax1.set_ylabel('matrix total nl its',color=color)
ax1.set_xlabel('time (days)')
ax1.tick_params(axis='y',labelcolor=color)

for iname in inputs:
    df = pd.read_csv(iname+'/matrix.csv')
    ax1.plot(df['time'] / 3600.0/ 24.0, df['total_nl_its'],\
             colours[iname], linestyle = lines[iname],\
             fillstyle='none', label=labels[iname])

ax2=ax1.twinx()
color='tab:blue'
ax2.set_ylabel('frac total nl its',color=color)
ax2.tick_params(axis='y',labelcolor=color)

for iname in inputs:
    df = pd.read_csv(iname+'/frac.csv')
    ax2.plot(df['time'] / 3600.0/ 24.0, df['total_nl_its'],\
             colours[iname], marker = markers[iname],\
             fillstyle='none', label=labels[iname])


ax1.set_ylim(0, 3000)
ax2.set_ylim(0, 3000)
# fig.tight_layout(pad=1)
# name='probability_'+str(temp)+'C_'+str(pressure)+'MPa_laromance.pdf'
# fig1.savefig(name, format='pdf', bbox_inches='tight', pad_inches=0.5)
#-------------------------------------------------------------------------------
fig1 = plt.figure()
ax1 = fig1.add_subplot(1,1,1)
color = 'tab:green'
ax1.set_ylabel('matrix nl its',color=color)
ax1.set_xlabel('step')
ax1.tick_params(axis='y',labelcolor=color)

for iname in inputs:
    df = pd.read_csv(iname+'/matrix.csv')
    ax1.plot(df['nl_its'],\
             colours[iname], linestyle = lines[iname],\
             fillstyle='none', label=labels[iname])

ax2=ax1.twinx()
color='tab:blue'
ax2.set_ylabel('frac nl its',color=color)
ax2.tick_params(axis='y',labelcolor=color)

for iname in inputs:
    df = pd.read_csv(iname+'/frac.csv')
    ax2.plot(df['nl_its'],\
             colours[iname], marker = markers[iname],\
             fillstyle='none', label=labels[iname])


# ax1.set_ylim(0, 2500)
# ax2.set_ylim(0, 2500)


plt.show()

# sys.exit(0)
