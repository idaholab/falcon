import tools

def test():
  tools.executeAppAndDiff(__file__,'thermo_mech_test.i',['out.e'])
