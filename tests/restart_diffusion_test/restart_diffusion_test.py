from options import *

test1 = { INPUT : 'restart_diffusion_test_steady.i',
         EXODIFF : ['steady_out.e']}
test2 = { INPUT : 'restart_diffusion_test_transient.i',
        EXODIFF : ['out.e'],
        PREREQ : ['test1'] }

