import tools

def test():
  tools.executeFalconAndDiff(__file__,'coupled_bc_test.i',['out.e'])
