from points2tets import *

# FUNCTION to FIND LOCATION
def findlocation(keyword):
    string = str(keyword)
    for s in list:
        if string in str(s):
            out=list.index(s)
    return out

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

if NullANS == '1':
    NullV=float(-10000) # Null Value
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
