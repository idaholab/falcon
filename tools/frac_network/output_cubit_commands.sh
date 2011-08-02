#!/bin/bash

export DYLD_LIBRARY_PATH=${DYLD_LIBRARY_PATH}:/Applications/Cubit-13.0/Cubit.app/Contents/MacOS/

python2.5 ./_output_cubit_commands.py | grep "^[[:lower:]]"














