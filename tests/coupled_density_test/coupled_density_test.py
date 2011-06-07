import tools

def test(dofs=0, np=0, n_threads=0):
 # pass
  tools.executeAppAndDiff(__file__,'coupled_density_test.i',['out.e'], dofs, np, n_threads)

try:
  from options import *

  test = { INPUT : 'coupled_density_test.i',
           EXODIFF : ['out.e']}

except:
  pass
