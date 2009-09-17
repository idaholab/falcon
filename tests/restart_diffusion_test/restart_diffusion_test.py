import tools

def test():
    tools.executeFalconAndDiff(__file__,'restart_diffusion_test_steady.i',['steady_out.e'])
    tools.executeFalconAndDiff(__file__,'restart_diffusion_test_transient.i',['out.e'])
