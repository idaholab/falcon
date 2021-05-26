#!/usr/bin/env python3
import sys
import os

# Locate MOOSE directory - first check if MOOSE_DIR env. is already set, if not, check for submodule
MOOSE_SUBMODULE_DIR = os.path.abspath(os.path.join(os.path.dirname(__name__), '..', 'moose'))
MOOSE_DIR = os.getenv('MOOSE_DIR', MOOSE_SUBMODULE_DIR)

# If MOOSE_DIR env. is not set and there's no submodule, check the parent directory of FALCON
if not os.path.isdir(MOOSE_DIR):
    MOOSE_DIR = os.path.abspath(os.path.join(os.path.dirname(__file__), '..', '..', 'moose'))
if not os.path.isdir(MOOSE_DIR):
    print('Failed to locate MOOSE, specify the MOOSE_DIR environment variable.')
    sys.exit(1)
os.environ['MOOSE_DIR'] = MOOSE_DIR

# Append the MOOSE python directory to path so that modules can be imported
MOOSE_PYTHON_DIR = os.path.join(MOOSE_DIR, 'python')
if MOOSE_PYTHON_DIR not in sys.path:
    sys.path.append(MOOSE_PYTHON_DIR)

# If using the MOOSE submodule, it needs to be initialized
if MOOSE_DIR == MOOSE_SUBMODULE_DIR:
    import mooseutils
    mooseutils.git_init_submodule(os.path.relpath(MOOSE_DIR))

# Import and run MooseDocs
from MooseDocs import main
if __name__ == '__main__':
    sys.exit(main.run())
