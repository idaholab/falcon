#!/usr/bin/env python
import sys, os 


x=os.getcwd()
y=os.path.join(x,'scripts')

sys.path.append(y)
import points2tets
