import tools

def test():
  tools.executeFalconAndDiff(__file__,'nodal_aux_var_test.i',['out.e'])
