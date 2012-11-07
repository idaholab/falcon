from options import *

test1 = { INPUT : 'DensityWater_PT_test.i',
         CSVDIFF : ['DensityWater_PT_test_out.csv'],
         TYPE : 'CSVDiff',
             REL_ERR : 1.5e-5}

test2 = { INPUT : 'DensitySteam_PT_test.i',
          CSVDIFF : ['DensitySteam_PT_test_out.csv'],
          TYPE : 'CSVDiff',
              REL_ERR : 5.5e-4}

test3 = { INPUT : 'EnthalpyWater_PT_test.i',
          CSVDIFF : ['EnthalpyWater_PT_test_out.csv'],
          TYPE : 'CSVDiff',
              REL_ERR : 2e-4}

test4 = { INPUT : 'EnthalpySteam_PT_test.i',
          CSVDIFF : ['EnthalpySteam_PT_test_out.csv'],
          TYPE : 'CSVDiff',
              REL_ERR : 2e-4}
