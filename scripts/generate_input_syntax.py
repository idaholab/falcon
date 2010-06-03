#!/usr/bin/env python
import sys, os
from socket import gethostname

# if we're running this from the build system we only want to run it once instead
# of on all four targets. So bitten passes the --helios-only option, then this
# script does nothing if it is not run on helios
if len(sys.argv) == 2 and sys.argv[1] == '--helios-only':
  if gethostname() != 'helios':
    print 'Syntax NOT generated because this is not helios'
    sys.exit(0)

# get the location of this application
app_path = os.path.abspath(os.path.dirname(sys.argv[0]))
# this script is actually in the scripts subdirectory, so go up a level
app_path += '/..'

# Set the name of the application here and moose directory relative to the application
app_name = 'falcon'
MOOSE_DIR = app_path + '/../moose'
# See if MOOSE_DIR is already in the environment instead
if os.environ.has_key("MOOSE_DIR"):
  MOOSE_DIR = os.environ['MOOSE_DIR']

sys.path.append(MOOSE_DIR + '/scripts/syntaxHTML')
import genInputFileSyntaxHTML

# this will automatically copy the documentation to the base directory
# in a folder named syntax
genInputFileSyntaxHTML.generateHTML(app_name, app_path, MOOSE_DIR)
