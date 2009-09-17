from nose.tools import *
from subprocess import *
import sys
import os

def falconDir():
  return os.path.dirname(os.path.dirname(os.path.abspath(__file__)))

def testDir(test_file):
  return os.path.dirname(test_file)

def delOldOutFiles(test_dir, out_files):
  for file in out_files:
    try:
      os.remove(os.path.join(test_dir,file))
    except:
      pass

def executeCommand(command):
  print 'Executing: ' + command

  p = Popen([command],stdout=PIPE,stderr=STDOUT, close_fds=True, shell=True)
  return p.communicate()[0]

def executableName():
  if os.environ.has_key('METHOD'):
    return 'falcon-' + os.environ['METHOD']
  
  return 'falcon-opt'

def executeFalcon(test_dir, input_file):
  saved_cwd = os.getcwd()
  os.chdir(test_dir)
  command = falconDir() + '/' + executableName() + ' -i ' + input_file
  stdout = executeCommand(command)
  print stdout
  os.chdir(saved_cwd)

def executeExodiff(test_dir, out_files):
  for file in out_files:
    command = 'exodiff -F 1e-11 -use_old_floor -t 5.5E-6 ' + os.path.join(test_dir,file) + ' ' + os.path.join(test_dir,'gold',file)
    print command
    stdout = executeCommand(command)
    print stdout
    if stdout.find('different') != -1 or stdout.find('ERROR') != -1 or stdout.find('command not found') != -1:
      assert False

def executeFalconAndDiff(test_file, input_file, out_files):
  test_dir = testDir(test_file)
  delOldOutFiles(test_dir, out_files)
  executeFalcon(test_dir, input_file)
  executeExodiff(test_dir, out_files)
