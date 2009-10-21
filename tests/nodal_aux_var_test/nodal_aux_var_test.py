import tools

def test():
  tools.executeAppAndDiff(__file__,'nodal_aux_var_test.i',['out.e'])
