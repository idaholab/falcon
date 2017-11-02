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

# READ FILE FROM INPUT
def readfile(filenameinput):
    with open(filenameinput) as f:
        out=[]
        for line in f:
            line = line.split()
            if line:
                line=[str(i) for i in line]  # convert to str
                out.append(line)
    return out
###################################################################
# FIND HOME DIRECTORY
from os.path import expanduser
os2=expanduser("~")

while True:
    print os.getcwd()
    print("-"*50)
    filename=raw_input('Enter Absolute Path of the File: ')
    try:
        f=open(filename)
        break
    except BaseException:
        print '\nERROR: INPUT FILE NOT FIND'

inname=filename
ename=inname[:-3]+"e"          # exodus name
csvname=inname[:-3]+".csv"      # CSV name
csvname2=inname[:-4]+".0.csv"   #CSV2name
print ("\n"+filename)
print "\nFile Found "

# Find Attributes Name from VTK file
data=readfile(filename)
attLine=[]
k=0
for i in range(len(data)):
    if "SCALARS" in data[i]:
        attLine.append([])
        attLine[k]=data[i]
        k+=1
attName=[]
for i in range(len(attLine)):
    attName.append([])
    attName[i]=attLine[i][1]
print "Finding Attributes in VTK file"
#----------------------------------------------------#
# ParaView PYTHON
print("-"*75)
#### import the simple module from the paraview
from paraview.simple import *
#### disable automatic camera reset on 'Show'
paraview.simple._DisableFirstRenderCameraReset()

# create a new 'Legacy VTK Reader'
examplevtk = LegacyVTKReader(FileNames=[inname])
print(filename+" is loaded into ParaView")



#RKP debugging 2017.11.02


#class SaveData:
#    def out(self):
#        print("it works")

# save data
SaveData(ename, proxy=examplevtk)
print(ename+" is saved in working directory")





# create a new 'ExodusIIReader'
examplee = ExodusIIReader(FileName=[ename])
print(ename+" is loaded in ParaView")

examplee.PointVariables = []
print "Loaded attributes into ParaView"
examplee.PointVariables = attName



#RKP 2017.101.02
# save data

SaveData(csvname, proxy=examplee, UseScientificNotation=1)
print(csvname+" is saved in working directory")






#### uncomment the following to render all views
# RenderAllViews()
# alternatively, if you want to write images, you can use SaveScreenshot(...)

print("ParaView Python Completed")
print("-"*75)
#----------------------------------------------------#
# MODIFY CSV FILE
with open(csvname2) as fin:
    lines = fin.readlines()
lines[0] = lines[0].replace('"','')

with open(csvname2, 'w') as fout:
    for line in lines:
        fout.write(line)

print("Quotes in CSV removed")

# Replace Name
os.rename(csvname2, csvname2.replace(".0",""))
print(csvname2+ " is renamed as "+ csvname2.replace(".0",""))
print("-"*34)
print("||Conversion to Exodus Completed||")
print("-"*34)
