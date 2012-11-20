from options import *

test1 = { INPUT : 'densitywater_pt_test.i',
         CSVDIFF : ['densitywater_pt_test_out.csv'],
         TYPE : 'CSVDiff',
             REL_ERR : 1.5e-5}

test2 = { INPUT : 'densitysteam_pt_test.i',
          CSVDIFF : ['densitysteam_pt_test_out.csv'],
          TYPE : 'CSVDiff',
              REL_ERR : 5.5e-4}

test3 = { INPUT : 'enthalpywater_pt_test.i',
          CSVDIFF : ['enthalpywater_pt_test_out.csv'],
          TYPE : 'CSVDiff',
              REL_ERR : 2e-4}

test4 = { INPUT : 'enthalpysteam_pt_test.i',
          CSVDIFF : ['enthalpysteam_pt_test_out.csv'],
          TYPE : 'CSVDiff',
              REL_ERR : 2e-4}
