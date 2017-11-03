#==============================================================================
#
#  Program: points2tets
#
#==============================================================================


# Modules
import os
import sys
import subprocess

# USER DEFINE FUNCTION

# FUNCTION to FIND LOCATION
def findlocation(keyword):
    string = str(keyword)
    for s in list:
        if string in str(s):
            out=list.index(s)
    return out

# COLORS CLASS
class bcolors:
    N='\033[0m'             #Normal
    BOLD = '\033[1m'        #Bold
    UNDERL = '\033[4m'      #Underline
    RED = '\033[91m'        #RED
    GREEN = '\033[42m'      #GREEN

# FUNCTION TO OUTPUT ERROR
def fileERROR(command,message):
    try:
        f= open(command)
    except BaseException:
        print bcolors.RED+'ERROR: ' +str(message) +' NOT FOUND'+bcolors.N
        sys.exit()

# FUNCTION REDUNDANCY
def redundancy(filetocheck):
    k=0
    for i in range(len(filetocheck)-1):

        #OPEN first file:
        with open(str(filetocheck[i])) as f:
            out=[]
            for line in f:
                line = line.split()
                if line:
                    line=[str(k) for k in line]  # convert to str
                    out.append(line)
        del out[0]

        #OPEN second file:
        with open(str(filetocheck[i])) as f:
            out2=[]
            for line in f:
                line = line.split()
                if line:
                    line=[str(k) for k in line]  # convert to str
                    out2.append(line)
        del out2[0]


        # Compare the 2 files
        for i in range(len(out)):
            for j in range(3):
                if out[i][j] == out2[i][j]:
                    continue
                else:
                    print('ERROR, XYZ DOES NOT MATCH')
                    print(os.path.basename(filetocheck[i+1]),' COORDINATES DOES NOT MATCH')
                    sys.exit()

    print(bcolors.GREEN+'REDUNDANCY CHECK OK'+bcolors.N)

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

# FIND FILEPATH FROM FILE NAME
def find(name, path):
    for root, dirs, files in os.walk(path):
        if name in files:
            return os.path.join(root, name)

# READ THE ATTRIBUTES OF THE FILE
def readatt(filepath):
    with open(filepath) as f:
        return f.readline()
    f.close()

# START OF THE SCRIPTS
#############################################################################

print("-"*70)
# GET CURRENT WORKING DIRECTORY AND MOVE INTO TEST DIRECTORY
os2=os.getcwd()   # OS2: Current working directory

# ASK FOR USER INPUT TO FILEFORMAT
while True:
    formatname=raw_input(bcolors.BOLD+"[1] ROCKWORKS OR [2] PETREL \nENTER FORMAT: " + bcolors.N)
    if formatname == '1':  # ROCKWORKS
        break
    elif formatname == '2':  # PETREL
        break
    else:
        print(bcolors.RED +"ERROR: FILE FORMAT NOT FOUND \n" + bcolors.N)

# ASK FOR USER INPUT TO THE FILE
while True:
    usrname=raw_input(bcolors.BOLD+'Enter Input File Name: '+bcolors.N)
    inname=os.path.join(os2,usrname)

    try:
        f= open(inname)
        break
    except BaseException:
        print bcolors.RED+'ERROR: INPUT FILE NOT FOUND'+bcolors.N

# FIND FILES
i=0
j=0
x=[]
for root, dirs, files in os.walk(os2, topdown=True):    # Find all files in the the dir
    for name in files:
        if usrname in name:
            j+=1
            x.append([])
            x[i-1]=(os.path.join(root, name))

f=inname.find(".")
outname=inname[0:f]+'.node'  # print .node absolute filepath

out=readfile(inname)

"""-------------------------------------------------------------------------------------------------------------"""
# FILE FORMAT MANUIPATION BASE ON FORMAT
# ROCKWORKS
if formatname == "1" :
    del out[0]

    n=usrname.find('.')
    string=usrname[:n]
    fileformat='.txt'
    # Find files
    i=0
    j=0
    x=[]
    for root, dirs, files in os.walk(os2, topdown=True):    # Find all files in the the dir
        for name in files:
            if string in name and fileformat in name:
                j+=1
                x.append([])
                x[i-1]=(os.path.join(root, name))


    # Call redundancy FUNCTION
    redundancy(x)

    # Extract Header Data
    i=0
    rheader=[]
    for i in range(len(x)):
        rheader.append([])
        rheader[i]=readatt(x[i])

    i=0
    kheader=[]
    for i in range(len(rheader)):
        string=rheader[i]
        kheader.append([])
        kheader[i]=string.replace('\r\n','')
    del rheader

    i=0
    header=[]
    for i in range(len(kheader)):
        string=kheader[i]
        header.append([])
        header[i]=string.replace('X Y Z ','')

    # Extract XYZ data
    with open(x[0]) as f:
            out=[]
            for line in f:
                line = line.split()
                if line:
                    line=[str(i) for i in line]  # convert to str
                    out.append(line)
            del out[0]


    xyz=[]
    for i in range(len(out)):
        xyz.append([])
        for j in range(3):
            xyz[i].append(0)
            xyz[i][j]=float(out[i][j])


    # Combine into one VTK
    att=[]
    for i in range(len(x)):

        # OPEN AND READ FILES
        with open(x[i]) as f:
            out=[]
            for line in f:
                line = line.split()
                if line:
                    line=[str(j) for j in line]  # convert to str
                    out.append(line)
            del out[0]

        # EXTRACT ATT VALUES
        att.append([])
        for k in range(len(out)):
            att[i].append(0)
            att[i][k]=str(out[k][3])

    # Transpose the att
    tatt=[list(x) for x in zip(*att)]

    # Combine xyz and attributes
    sourceout=[]
    for i in range(len(xyz)):
        sourceout.append([])
        sourceout[i]=xyz[i]+tatt[i]

    with open (outname,"w") as f:
        for i in range(len(sourceout)):
            for j in range(len(sourceout[0])):
                f.write(str(sourceout[i][j]))
                f.write(' ')
            f.write('\n')

    inname=outname
    out=readfile(inname)

# PETREL
elif formatname == "2":

    while True:
        SIname=raw_input(bcolors.BOLD+"Standard Unit[1] or GeoPhysics Unit[2]: "+bcolors.N)
        if SIname == '1':  #Standard Unit
            break
        elif SIname == '2':  # GeoPhysics Unit
            #Convert Unit
            out=readfile(inname)
            list = out
            i=0
            header=[]
            for s in list:
                if 'FLOAT' in str(s):
                    header.append(0)
                    header[i]=list.index(s)
                    i+=1

            pheader=[]
            for i in range(len(header)):
                pheader.append(0)
                pheader[i]=out[header[i]]

            headername=[]
            for i in range(len(pheader)):
                string=pheader[i][0]
                headername.append([])
                headername[i]=string.replace('FLOAT,','')

            for i in range(len(header)):
                if "NEUTRON_POROSITY" in str(headername[i]):
                    Neutron=i+3
                if "DENSITY" in str(headername[i]):
                    Density=i+3
                if "GAMMA" in str(headername[i]):
                    Gamma=i+3

            i=0
            for i in range(len(out)):
                if "END" in out[i]:
                    start=i
                    break

            NumRows=len(out)
            NumCol=len(out[start+1])

            for i in range(start+1,NumRows):
                for j in range(NumCol):
                    if float(out[i][j])==float(-999):

  #RKP need to skip any lines that have a -999, it is a key value for points outside the active simulation area
                       print "Found", (out[i][j]), "skipping line"
                       break
                    else:
                        if j==Neutron:  # Convert form Unit value to Percentage(%)
                            out[i][j]=float(out[i][j])*0.01
                        if j==Density:  # Convert g/cm^3 to kg/m^3
                            out[i][j]=float(out[i][j])*1000

            with open(inname,"w") as f:
                for i in range(len(out)):
                    for j in range(len(out[i])):
                        f.write(str(out[i][j]))
                        f.write(' ')
                    f.write('\n')
            print(bcolors.BOLD+'Converted to standard Unit'+bcolors.N)
            break
        else:
            print(bcolors.RED +"ERROR: Wrong Input" + bcolors.N)
    list = out

    locat=int(findlocation('END'))
    headlocat=[]
    i=0
    string = 'FLOAT'
    for s in list:
        if string in str(s):
            headlocat.append(0)
            headlocat[i]=list.index(s)
            i+=1

    pheader=[]
    for i in range(len(headlocat)):
        pheader.append(0)
        pheader[i]=out[headlocat[i]]

    header=[]
    for i in range(len(pheader)):
        string=pheader[i][0]
        header.append([])
        header[i]=string.replace('FLOAT,','')

    del out[0:locat+1]


"""-------------------------------------------------------------------------------------------------------------"""

# NUMBER OF ROWS AND COLUMNS
NumsRows=len(out)
NumsColu=(len(out[0]))

sys.stdout.write('Reading Input File'+'.'*15)

# ORGANIZE DATA INTO COLUMNS AND ROWS
x=[]
for j in range(NumsRows):
    x.append([])
    for i in range(NumsColu):
        x[j].append(0)
        x[j][i]=out[j][i]

print(bcolors.BOLD+"[DONE]"+bcolors.N)

# ADD NUMBERING TO THE FRONT OF EACH NODES
for i in range(NumsRows):
    x[i].insert(0,i+1)

# OUTPUT FILES WITH ADDED HEADER FOR INPUT TO TETGEN
sys.stdout.write('Convert to .node'+"."*17 )

with open(outname, "w") as f:
    M=(str(NumsRows)+' 3 1 1''\n')
    f.write(M)
    for j in range(len(x)):
        for i in range(len(x[0])):
                L=(str(x[j][i])+' ')
                f.write(L)
        f.write('\n')

# STATE OUTPUT
print(bcolors.BOLD+"[DONE]"+bcolors.N)
print 'Number of Nodes:',NumsRows
"""-------------------------------------------------------------------------------------------------------------"""

### CALL PROCESS TO RUN TETGEN
print(bcolors.BOLD+"CALLING TETGEN..."+bcolors.N)
print("-"*70)

#FIND TETGEN DIR
from os.path import expanduser
os5=expanduser("~")
#os5=os.path.dirname(os.path.dirname(os2)
tetgen='tetgen'

# Find the path of tetgen
pathtotetgen=find(tetgen,os5)

output=(pathtotetgen + " -kNEF " + outname)

subprocess.call(output,shell=True)
print(bcolors.GREEN+'Tetgen OK'+bcolors.N)
"""-------------------------------------------------------------------------------------------------------------"""

outname2=outname[:-4]+'1.vtk'

if formatname == '1':
    NullV=int(-10000)
elif formatname == '2':
    NullV=int(-998)

# NUMBER OF ATTRIBUTES (subtracting XYZ)
NumsA=NumsColu-3

# FORMAT ATTRIBUTES
x=[]
for i in range(NumsRows):
    x.append([])
    for j in range(NumsA):
        x[i].append(0)
        if float(out[i][j])<NullV:
            x[i][j]=-1;
        else:
            x[i][j]=out[i][j+3]

print("-"*70)
print("Writing Attributes...")

# OUTPUT INTO VTK
with open (outname2, "a+") as f:
    f.write('\n')
    M= 'POINT_DATA ' + str(NumsRows)+ '\n'
    f.write(M)
    for j in range(NumsA):
        M='SCALARS '+header[j]+' float \nLOOKUP_TABLE default \n'
        f.write(M)
        for i in range(NumsRows):
            if float(x[i][j])<NullV:
                f.write('-1.0');
                f.write('\n')
            else:
                f.write(str(x[i][j]))
                f.write('\n')
        print("Writing Attributes ["+str(j+1)+']')
    print(bcolors.BOLD+"[DONE]"+bcolors.N)

# RENAME OUTPUT FILE
os.rename(outname2, outname2.replace(".1",""))
print("-"*70)
print(bcolors.GREEN+bcolors.BOLD+"Finish"+bcolors.N)
print("Output VTK file: " +outname2.replace(".1",""))
print("-"*70)
