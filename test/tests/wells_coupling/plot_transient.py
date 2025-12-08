# Plots the results produced by both wells_main.i and fracs_main.i.

import sys
import os
import matplotlib.pyplot as plt

app_dir = os.path.abspath(os.path.join(os.path.dirname(__file__), '..', '..', '..'))
app_python_dir = os.path.join(app_dir, 'python')
if app_python_dir not in sys.path:
  sys.path.append(app_python_dir)

from falcon_utilities import addTHMPythonPath
addTHMPythonPath(app_dir)
from thm_utilities import readCSVFile

def makePlot(output_dir, var, y_label, scale):
  data_fracs = readCSVFile(output_dir + '/fracs.csv')
  data_wells = readCSVFile(output_dir + '/wells.csv')

  plt.figure(figsize=(8, 6))
  plt.rc('text', usetex=True)
  plt.rc('font', family='sans-serif')
  ax = plt.subplot(1, 1, 1)
  ax.get_yaxis().get_major_formatter().set_useOffset(False)
  plt.xlabel("Time [s]")
  plt.ylabel(y_label)
  inj1 = data_fracs['mass_rate_inj1']
  inj2 = data_fracs['mass_rate_inj2']
  inj3 = data_fracs['mass_rate_inj3']
  total_inj = inj1 + inj2 + inj3
  pro1 = data_fracs['mass_rate_pro1']
  pro2 = data_fracs['mass_rate_pro2']
  pro3 = data_fracs['mass_rate_pro3']
  total_pro = pro1 + pro2 + pro3
  plt.plot(data_fracs['time'], total_inj,  linestyle='-', marker='', color='cornflowerblue', label="Total Injection, Fracs")
  plt.plot(data_fracs['time'], -total_pro,  linestyle='-', marker='', color='indianred', label="Total Production, Fracs")
  plt.plot(data_wells['time'], data_wells['mass_rate_inlet'],  linestyle='--', marker='', color='cornflowerblue', label="Total Injection, Wells")
  plt.plot(data_wells['time'], data_wells['mass_rate_outlet'],  linestyle='--', marker='', color='indianred', label="Total Production, Wells")
  ax.legend()
  plt.tight_layout()
  plt.savefig(output_dir + '/' + var + '_transient.png', dpi=300)

makePlot('outputs/fracs_main', 'mass_rate', 'Mass Flow Rate [kg/s]', 1)
makePlot('outputs/wells_main', 'mass_rate', 'Mass Flow Rate [kg/s]', 1)
