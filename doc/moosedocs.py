#!/usr/bin/env python3
import sys
import os

# Locate MOOSE directory
MOOSE_DIR = os.getenv('MOOSE_DIR', os.path.abspath(os.path.join(os.path.dirname(os.path.realpath(__file__)), '..', '..')))
if not os.path.exists(MOOSE_DIR):
    MOOSE_DIR = os.path.abspath(os.path.join(os.path.dirname(__name__), '..', '..', 'moose'))
if not os.path.exists(MOOSE_DIR):
    print('Failed to locate MOOSE, specify the MOOSE_DIR environment variable.')
    sys.exit(1)
os.environ['MOOSE_DIR'] = MOOSE_DIR

# Locate FALCON root in environment variables, otherwise, set path relative to 'falcon/doc'
# if 'FALCON_DIR' not in os.environ:
#     os.environ['FALCON_DIR'] = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))

# Append MOOSE python directory
MOOSE_PYTHON_DIR = os.path.join(MOOSE_DIR, 'python')
if MOOSE_PYTHON_DIR not in sys.path:
    sys.path.append(MOOSE_PYTHON_DIR)

from MooseDocs import main
if __name__ == '__main__':
    sys.exit(main.run())
