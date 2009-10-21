import tools

def test():
  tools.executeAppAndDiff(__file__,'transient_thermo_mech_test.i',['out.e'])
