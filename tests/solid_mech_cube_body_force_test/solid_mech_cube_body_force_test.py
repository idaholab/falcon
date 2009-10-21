import tools

def test():
  tools.executeAppAndDiff(__file__,'solid_mech_cube_body_force_test.i',['out.e'])
