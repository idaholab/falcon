import itertools
import sys

############################################################
#Ask User the corresponding Bound
def AskUSER(xbound):
    num=float(raw_input(bcolors.BOLD+xbound))
    return num

# COLORS CLASS
class bcolors:
    N='\033[0m'             #Normal
    BOLD = '\033[1m'        #Bold
    UNDERL = '\033[4m'      #Underline
    RED = '\033[91m'        #RED
    GREEN = '\033[42m'      #GREEN

############################################################
print "-"*32
print bcolors.BOLD+'||Enter the Limit of each axis||'+bcolors.N
print "-"*32
xmin=AskUSER('X-Min: ')
xmax=AskUSER('X-Max: ')

ymin=AskUSER('Y-Min: ')
ymax=AskUSER('Y-Max: ')

zmin=AskUSER('Z-Min: ')
zmax=AskUSER('Z-Max: ')

##xmin=324547.91
##xmax=346279.17
##
##ymin=4256315.96
##ymax=4271658.24
##
##zmin=-5000
##zmax=2770.56

#Ask User for number of node
numofnode=int(input(bcolors.BOLD+'Number of Node: '))
print "-"*32
print bcolors.BOLD+"Creating list of each limit..............",
# Create list of number each limit
xlist=[]
for i in range(numofnode):
    xlist.append([])
    xincr=(xmax-xmin)/(numofnode-1)
    xlist[i]=xmin+i*xincr

ylist=[]
for i in range(numofnode):
    ylist.append([])
    yincr=(ymax-ymin)/(numofnode-1)
    ylist[i]=ymin+i*yincr

zlist=[]
for i in range(numofnode):
    zlist.append([])
    zincr=(zmax-zmin)/(numofnode-1)
    zlist[i]=zmin+i*zincr
print bcolors.GREEN+"DONE"+bcolors.N
# Mix X,Y, and Z together into one combine list
a=[xlist,ylist,zlist]

# Get every combination of the 3 lists
print bcolors.BOLD+"Iterating though List....................",
b=list(itertools.product(*a))
print bcolors.GREEN+"DONE"+bcolors.N

# Write to out.txt, and remove commas and brackets
with open('out.txt','w') as f:
    for i in range(len(b)):
        m=str(b[i])
        n=m[1:-1]
        n=n.replace(',',' ')
        f.write(n)
        f.write('\n')

print bcolors.BOLD+"Writing to output file...................."+bcolors.GREEN+"DONE"+bcolors.N
