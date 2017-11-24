from points2tets import *

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
# print out XYZ
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

if NullANS == '1':
    NullV=float(-999) # Null Value
    NullR=[]  # Null Value Data
    k=0
    for i in range(len(out)):
        for j in range(len(out[0])):
            if float(out[i][j])==NullV:
                NullR.append([])
                NullR[k]=i
                k=k+1
    for i in range(len(NullR)):
        out.pop(i)
