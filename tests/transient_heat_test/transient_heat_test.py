import tools

def test():
  tools.executeAppAndDiff(__file__,'transient_heat_test.i',['out.e'])
