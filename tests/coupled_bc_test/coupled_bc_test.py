import tools

def test():
  tools.executeAppAndDiff(__file__,'coupled_bc_test.i',['out.e'])
