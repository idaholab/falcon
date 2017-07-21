#==============================================================================
#
#  Program:   vtk2exodus
#
#==============================================================================

import os,sys

# FIND FILES
def find(name,path):
    for root, dirs, files in os.walk(path):
        files = [f for f in files if not f[0] == '.']   # Ignore hidden folder
        dirs[:] = [d for d in dirs if not d[0] == '.']  # Ignore hidden files
        if name in files:
            return os.path.join(root, name)

# FIND HOME DIRECTORY
from os.path import expanduser
os2=expanduser("~")

# ENTER INPUT FILENAME
while True:
    filename=raw_input("Enter filename:")

    try:
        f=find(filename,os2)
        open(f)
        break
    except BaseException:
        print '\n ERROR: INPUT FILE NOT FIND'

inname=find(filename,os2)
ename=inname[:-3]+"e"          # exodus name
csvname=inname[:-3]+"csv"      # CSV name

# PARAVIEW PYTHON
#----------------------------------------------------#

#### import the simple module from the paraview
from paraview.simple import *
#### disable automatic camera reset on 'Show'
paraview.simple._DisableFirstRenderCameraReset()

# create a new 'Legacy VTK Reader'
examplevtk = LegacyVTKReader(FileNames=[inname])

# save data
SaveData(ename, proxy=examplevtk)

# create a new 'ExodusIIReader'
examplee = ExodusIIReader(FileName=[ename])
examplee.PointVariables = []

# save data
SaveData(csvname, proxy=examplee)

#### uncomment the following to render all views
# RenderAllViews()
# alternatively, if you want to write images, you can use SaveScreenshot(...).

print("\n Done")
