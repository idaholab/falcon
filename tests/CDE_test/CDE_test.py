import tools

def test(dofs=0, np=0, n_threads=0):
 # pass
  tools.executeAppAndDiff(__file__,'pressure_temp_1d_x_test.i',['out.e'], dofs, np, n_threads)
