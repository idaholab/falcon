import sys
import os

def addTHMPythonPath(abs_app_dir):
  # find MOOSE directory
  moose_dir = os.getenv('MOOSE_DIR', os.path.join(abs_app_dir, 'moose'))
  if not os.path.exists(moose_dir):
    moose_dir = os.path.join(abs_app_dir, '..', 'moose')
  if not os.path.exists(moose_dir):
    raise Exception('Failed to locate MOOSE: please specify the MOOSE_DIR environment variable.')

  # add THM python path
  thm_python_dir = os.path.join(moose_dir, 'modules', 'thermal_hydraulics', 'python', 'moose_thm')
  if thm_python_dir not in sys.path:
    sys.path.append(thm_python_dir)
