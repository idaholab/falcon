import tools

def test():
  tools.executeAppAndDiff(__file__,'2d_diffusion_test.i',['out.e'])
