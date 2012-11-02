from options import *

test1 = { INPUT : 'DensityWater_PT_test.i',
         CSVDIFF : ['DensityWater_PT_test_out.csv'],
         REL_ERR : 5e-5,
         TYPE : 'CSVDIFF'}

test2 = { INPUT : 'DensitySteam_PT_test.i',
          CSVDIFF : ['DensitySteam_PT_test_out.csv'],
          REL_ERR : 8e-4,
          TYPE : 'CSVDIFF'}

test3 = { INPUT : 'EnthalpyWater_PT_test.i',
          CSVDIFF : ['EnthalpyWater_PT_test_out.csv'],
          REL_ERR : 5e-4,
          TYPE : 'CSVDIFF'}

test4 = { INPUT : 'EnthalpySteam_PT_test.i',
          CSVDIFF : ['EnthalpySteam_PT_test_out.csv'],
          REL_ERR : 5e-4,
          TYPE : 'CSVDIFF'}
